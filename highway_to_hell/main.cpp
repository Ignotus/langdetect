#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <locale>
#include <cmath>

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
    std::cout << "Process: " << file_name << std::endl;
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

void load_dict(const std::string& file, std::map<std::wstring, unsigned long>& map,
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

void detect(const std::string& filename)
{
    std::vector<std::map<std::wstring, unsigned long>> trigrams_frequencies(7);
    // Load dict
    const char *countries[] = {"en", "ru", "bg", "uk", "de", "af", "pl"}; 
    unsigned long sum[7];
    for (int i = 0; i < 7; ++i)
    {
        std::wcout << L"Loading dict .. " << std::endl;
        load_dict(countries[i], trigrams_frequencies[country_id(countries[i])], sum[i]);
        std::wcout << L"Dict size: " << trigrams_frequencies[country_id(countries[i])].size() << std::endl; 
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
            const std::map<std::wstring, unsigned long>& m = trigrams_frequencies[j];

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
    

    std::wcout << L"{";
    for (int i = 0; i < 6; ++i)
    {
        std::wcout << countries[i] << L": " << perplexity[i] / sump  << L", ";
    }

    std::wcout << countries[6] << L": " << perplexity[6] / sump << L"}" << std::endl;

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

    if (strcmp(argv[1], "-train"))
        return -1;


    boost::filesystem::path directory(argv[2]);

    std::vector<std::map<std::wstring, unsigned long>> trigrams_frequencies(7);

    for (boost::filesystem::directory_iterator it(directory), end_it; it != end_it; ++it)
    {
        if (!boost::filesystem::is_directory(it->status()))
        {
            const boost::filesystem::path& file_path = it->path();
//            std::cout << "Process: " << file_path.string() << std::endl;
            if (file_path.extension().string() == ".txt")
            {
                const std::string& file_name = file_path.filename().string();
                const std::string& country = get_country(file_name);

                auto& m = trigrams_frequencies[country_id(country)];


                //TODO: Process each file
                std::list<std::wstring> word_list;
                get_words(file_path.string(), word_list);
                
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
                    
                    // Process token

                }
            }
        }
    }

    const char *countries[] = {"en", "ru", "bg", "uk", "de", "af", "pl"}; 

    int i = 0;
    for (const auto& p : trigrams_frequencies)
    {
        std::wfstream file(countries[i], std::fstream::out);
        
        std::vector<std::pair<std::wstring, unsigned long> > ll;
        ll.reserve(p.size());
        for (const auto& p1 : p)
        {
            ll.push_back(std::make_pair(p1.first, p1.second));

            //file << p1.first << L" " << p1.second << std::endl;
        }

        std::sort(ll.begin(), ll.end(), [](const std::pair<std::wstring, unsigned long>& f,
                                           const std::pair<std::wstring, unsigned long>& s)
                                           {
                                            return f.second > s.second;
                                           });

        for (const auto& p1 : ll)
            file << p1.first << L" " << p1.second << std::endl;


        ++i;
    }


    return 0;
}
