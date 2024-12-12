#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

#include <numeric>
#include <map>

int main()
{

    std::vector<int> left;
    std::vector<int> right;

    std::istream* inStream = &std::cin;

    std::ifstream file( std::string(InputFilePath));
    if(file.is_open())
    {
        inStream = &file;
    }

    Utility::Parse(*inStream, left, right);

    std::map<int, int> rightMap;

    for(const auto r:right)
    {
        if(!rightMap.contains(r))
        {
            rightMap[r] = 1;
        }
        else
        {
            ++rightMap[r];
        }
    }

    int sum = 0;
    for(const auto l:left)
    {
        if(rightMap.contains(l))
        {
            sum+=l*rightMap[l];
        }
    }

    std::cout << sum << std::endl;;

    return 0;
}