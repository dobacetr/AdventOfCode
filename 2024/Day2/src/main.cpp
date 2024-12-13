#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

template<Utility::Parse::IsContainer T> bool IsSafeLevels(const T& InLevels)
{
    // branchless version
    int prevDiff = 0;
    bool result = true;
    for(size_t idx = 1; idx < InLevels.size(); ++idx)
    {
        int diff = InLevels[idx]-InLevels[idx-1];

        // Monotonous, difference does not change sign
        const bool bSameSign = prevDiff*diff>=0;
        result &= bSameSign;

        // safe change, 1 trough 3
        const bool bSafeChange = diff*diff <= 9 && diff != 0;
        result &= bSafeChange;

        prevDiff = diff;
    }

    return result;
}

int main()
{
    // Read From file if exists
    std::istream* inStream = &std::cin;

    std::ifstream file( std::string(InputFilePath));
    if(file.is_open())
    {
        inStream = &file;
    }

    // Define vectors of values to be read from input

    // Parse rows into arrays
    std::vector<std::vector<int>> allLevels = Utility::Parse::Parse<int>(*inStream);

    size_t safeCount = 0;
    for(const auto& levels:allLevels)
    {
        safeCount += static_cast<int>(IsSafeLevels(levels));
    }

    std::cout << safeCount << std::endl;

    return 0;
}