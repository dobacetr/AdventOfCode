#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

template<Utility::Parse::IsContainer T> bool IsSafeLevels(const T& InLevels, size_t& outBadIndex, const size_t ignoreIdx=SIZE_MAX)
{
    // branchless version
    int prevDiff = 0;
    bool bIsSafe = true;
    outBadIndex = 0;

    const bool bFirstIsIgnored = ignoreIdx == 0;

    for(size_t idx = 1+bFirstIsIgnored; idx < InLevels.size(); ++idx)
    {
        const bool bIsIgnoreIdx = idx == ignoreIdx;
        const bool bPrevIsIgnoreIdx = idx-1==ignoreIdx;

        int diff = InLevels[idx]-InLevels[idx-1-bPrevIsIgnoreIdx];

        // Monotonous, difference does not change sign
        const bool bSameSign = prevDiff*diff>=0;

        // safe change, 1 trough 3
        const bool bSafeChange = diff*diff <= 9 && diff != 0;
        const bool bLevelIsSafe = bSameSign && bSafeChange;

        // Ignore Idx are always safe
        bIsSafe&=(bLevelIsSafe||bIsIgnoreIdx);

        // only record if not ignored
        outBadIndex+=!bIsSafe*idx*(outBadIndex==0)*(!bIsIgnoreIdx);
        prevDiff = bIsSafe*diff+!bIsSafe*prevDiff;
    }

    return bIsSafe;
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
    size_t badIndex = 0;
    for(const auto& levels:allLevels)
    {
        bool bIsSafeLevel = IsSafeLevels(levels, badIndex);
        if(!bIsSafeLevel)
        {
            const size_t ignoreIdx = badIndex;
            bIsSafeLevel = IsSafeLevels(levels, badIndex, ignoreIdx);
            if(!bIsSafeLevel&&ignoreIdx>0)
            {
                bIsSafeLevel = IsSafeLevels(levels, badIndex, ignoreIdx-1);
            }
        }
        safeCount+=bIsSafeLevel;
    }

    std::cout << safeCount << std::endl;

    return 0;
}