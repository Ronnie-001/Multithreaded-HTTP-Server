#include "m_strings.h"
#include <algorithm>
#include <iostream>
#include <iterator>

namespace cerberus
{
    namespace string 
    {
        int caseInsensitiveSearch(const std::string& str1, const std::string& str2)
        {
            auto it = std::search(str1.begin(), str1.end(),
                                  str2.begin(), str2.end(),
                                
                                    // Comparison function 
                                    [](const char a, const char b) {
                                        return std::tolower(a) == std::tolower(b);
                                    }

                                 );
            
            if (it != str1.end()) {
                return std::distance(str1.begin(), it);
            }

            return static_cast<int>(std::string::npos);
        }

        void printRawCharacters(const std::string& str) 
        {
            for (char c : str) {
                switch (c) {
                    case '\n': std::cout << "\\n"; break;
                    case '\t': std::cout << "\\t"; break;
                    case '\r': std::cout << "\\r"; break;
                    case '\\': std::cout << "\\\\"; break;
                    default:   std::cout << c;    break;
                }

            }
        }
    }
}
