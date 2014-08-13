#include <iostream>

int main()
{
  char prev_c;
  char c;

  prev_c = std::cin.get();
  c = std::cin.get();

  while (std::cin.good()) 
  {
    std::cout << prev_c << c << std::endl;  
    prev_c = c;
    c = std::cin.get();
    if (c == '\n')
    {
      prev_c = std::cin.get();
      c = std::cin.get();
    }
  } 

  return 0;
}
