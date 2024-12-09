#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

#include <numeric>

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

    std::sort(left.begin(),left.end());
    std::sort(right.begin(),right.end());

    std::vector<int> distances;
    distances.reserve(left.size());

    for(size_t idx = 0; idx < left.size(); ++idx)
    {
        distances.push_back( std::abs(left[idx] - right[idx]));
    }

    //Utility::PrintAll(std::cout, distances);

    std::cout << std::reduce(distances.begin(), distances.end());

    return 0;
}