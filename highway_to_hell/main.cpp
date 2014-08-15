#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include <locale>
#include <cmath>

#include <omp.h>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

std::string get_country(const std::string& file_name)
{
    std::string res(file_name.substr(0, 2));

    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

int country_id(const std::string& country)
{
    if (country == "en")
        return 0;
    if (country == "ru")
        return 1;
    if (country == "bg")
        return 2;
    if (country == "uk")
        return 3;
    if (country == "de")
        return 4;
    if (country == "af")
        return 5;
    return 6;
}

void get_words(const std::string& file_name, std::list<std::wstring>& word_list)
{
  //#pragma omp critical
    //std::cout << "Process: " << file_name << std::endl;
    
    std::wfstream in(file_name, std::fstream::in);
    std::wstring line;

    const boost::wregex rx(L"(</?doc[^>]*>)|((\\s|[<>“*+~—`′^$@_«»=*-\\\\/{};~–|\\(\\),.&#:\"\\]\\[%-'!])+)");

    while (std::getline(in, line))
    {
        //std::wcout << line << std::endl;
        // 
        
        boost::wsregex_token_iterator first(line.begin(), line.end(), rx, -1), last;
        word_list.insert(word_list.end(), first, last);
    }
}

void load_dict(const std::string& file, std::unordered_map<std::wstring, unsigned long>& map,
               unsigned long& sum)
{
    std::wfstream in(file, std::fstream::in);
    std::wstring line;

    const boost::wregex rx(L"\\s+");

    sum = 0;

    while (std::getline(in, line))
    {
        boost::wsregex_token_iterator first(line.begin(), line.end(), rx, -1);
        const std::wstring key = std::wstring(*first);
     //   std::wcout << key << L" " << key.size() << L" ";
        ++first;
        //std::wcout << *first << std::endl;
        unsigned long value = boost::lexical_cast<unsigned long>(std::wstring(*first));
        map[key] = value;
       // std::wcout << value << std::endl;
        sum += value;
    }
}

void detect(const std::string& filename, bool one_answer = false)
{
    std::vector<std::unordered_map<std::wstring, unsigned long>> trigrams_frequencies(7);
    // Load dict
    const char *countries[] = {"en", "ru", "bg", "uk", "de", "af", "pl"}; 
    unsigned long sum[7];
    for (int i = 0; i < 7; ++i)
    {
        //std::wcout << L"Loading dict .. " << std::endl;
        load_dict(countries[i], trigrams_frequencies[country_id(countries[i])], sum[i]);
        //std::wcout << L"Dict size: " << trigrams_frequencies[country_id(countries[i])].size() << std::endl; 
    }

    std::list<std::wstring> word_list;
    get_words(filename, word_list);

    double perplexity[7] = {0, 0, 0, 0, 0, 0, 0};

    // Calculate perplexity
    std::set<std::wstring> unique_trigram;
    for (const std::wstring& token : word_list)
    {
        if (token.size() < 3)
            continue;
        for (int i = 0; i < token.size() - 2; ++i)
        {
            std::wstring trigram = L"   ";
            trigram[0] = token[i];
            trigram[1] = token[i + 1];
            trigram[2] = token[i + 2];
            std::transform(trigram.begin(), trigram.end(), trigram.begin(),std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), std::locale("")));

            unique_trigram.insert(trigram);

        }
    }

    for (const std::wstring& trigram : unique_trigram)
    {
        for (int j = 0; j < 7; ++j)
        {
            const std::unordered_map<std::wstring, unsigned long>& m = trigrams_frequencies[j];

            auto it = m.find(trigram);
            double frequency = 0;
            if (it != m.end())
            {
//                    std::wcout << L"found: " << trigram << " " << it->second << std::endl;
                frequency = static_cast<double>(it->second) / sum[j];
                perplexity[j] += frequency * std::log2(frequency);
            }
        }

    }
    
    double sump = 0;
    for (int i = 0; i < 7; ++i)
    {
        perplexity[i] = std::pow(2, - perplexity[i]);
        sump += perplexity[i];
    }
    

    if (!one_answer)
    {
        std::wcout << L"{";
        for (int i = 0; i < 6; ++i)
        {
            std::wcout << countries[i] << L": " << perplexity[i] / sump  << L", ";
        }

        std::wcout << countries[6] << L": " << perplexity[6] / sump << L"}" << std::endl;
    }
    else
    {
        int max_index = 0;
        double max_value = 0;
        for (int i = 0; i < 7; ++i)
        {
            if ((perplexity[i] / sump) > max_value)
            {
                max_index = i;
                max_value = perplexity[i] / sump;
            }
        }
        
        std::cout << countries[max_index] << std::endl;
    }
    
}

void train(const std::string& file_name)
{
    boost::filesystem::path directory(file_name);

    std::vector<std::unordered_map<std::wstring, unsigned long>> trigrams_frequencies(7);
    
    std::vector<std::vector<boost::filesystem::path>> files(7);
    for (boost::filesystem::directory_iterator it(directory), end_it; it != end_it; ++it)
    {
        if (!boost::filesystem::is_directory(it->status()))
        {
            const boost::filesystem::path& file_path = it->path();
            if (file_path.extension().string() == ".txt")
            {
              const std::string& country = get_country(file_path.filename().string());
              files[country_id(country)].push_back(file_path);
            }
        }
    }
    
    
    omp_set_num_threads(4);
    
#pragma omp parallel for
    for (int k = 0; k < 7; ++k)
    {
          for (const boost::filesystem::path& file : files[k])
          {
              const std::string& file_name = file.filename().string();

              auto& m = trigrams_frequencies[k];

              std::list<std::wstring> word_list;
              get_words(file.string(), word_list);
              
              for (const std::wstring& token : word_list)
              {
                  if (token.size() < 3)
                      continue;
                  for (int i = 0; i < token.size() - 2; ++i)
                  {
                      std::wstring trigram = L"   ";
                      trigram[0] = token[i];
                      trigram[1] = token[i + 1];
                      trigram[2] = token[i + 2];
                      std::transform(trigram.begin(), trigram.end(), trigram.begin(), std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), std::locale("")));

                      auto it = m.find(trigram);  
                      if (it == m.end())
                          m[trigram] = 1;
                      else
                          ++m[trigram];
                  }
              }
          }
    }

    const char *countries[] = {"en", "ru", "bg", "uk", "de", "af", "pl"}; 

#pragma omp parallel for
    for (int i = 0; i < static_cast<int>(trigrams_frequencies.size()); ++i)
    {
        std::wfstream file(countries[i], std::fstream::out);
        
        std::vector<std::pair<std::wstring, unsigned long> > ll;
        ll.reserve(trigrams_frequencies[i].size());
        for (const auto& p1 : trigrams_frequencies[i])
            ll.push_back(std::make_pair(p1.first, p1.second));

        std::sort(ll.begin(), ll.end(), [](const std::pair<std::wstring, unsigned long>& f,
                                           const std::pair<std::wstring, unsigned long>& s)
                                           {
                                            return f.second > s.second;
                                           });

        for (const auto& p1 : ll)
            file << p1.first << L" " << p1.second << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        return -1;
    std::locale::global(std::locale(""));

    if (!strcmp(argv[1], "-detect"))
    {
        detect(argv[2]); 
        return 0;
    }
    
    if (!strcmp(argv[1], "-detect-one-answer"))
    {
        detect(argv[2], true);
        return 0;
    }

    if (strcmp(argv[1], "-train"))
        return -1;

    train(argv[2]);

    return 0;
}
