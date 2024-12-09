#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include "main.h"

using namespace std;

const char emptyChar = '.';
const string digits = "0123456789";

bool IsSpecialCharacter(const char c){
    bool notDigit = digits.find(c) == digits.npos;
    bool notEmpty = c != emptyChar;
    return notDigit && notEmpty;
    
}

std::vector<int> ExtractGearNums(const size_t iStart, const size_t iEnd, const std::string &line)
{
    std::vector<int> gearNums;

    //cout << "Searching from " << iStart << " to " << iEnd << endl;

    if (line.size() > 0)
    {
        // Find the first digit adjacent to *
        size_t iNumPos = line.find_first_of(digits.c_str(), iStart);

        while (iNumPos <= iEnd)
        {
            //cout << "Digit found at " << iNumPos << endl;

            // Find the first digit in the sequence of digits
            size_t iPrevChar = line.find_last_not_of(digits, iNumPos);
            if(iPrevChar!=line.npos){
                iNumPos = iPrevChar + 1;
            }
            else{
                // Starts at the beginning of the line
                iNumPos = 0;
            }

            //cout << "Start digit found at " << iNumPos << endl;

            // Find the last digit in the sequence of digits
            size_t iNextChar = line.find_first_not_of(digits, iNumPos);
            size_t iNumEnd;
            if( iNextChar != line.npos ){
                iNumEnd = iNextChar - 1;
            }
            else{
                // Ends at the end of the line
                iNumEnd = line.size()-1;
            }

            //cout << "End digit found at " << iNumEnd << endl;

            string numStr = line.substr(iNumPos, iNumEnd - iNumPos + 1);

            //cout << "Potential gear number: " << numStr << endl;

            int num = stoi(numStr);
            gearNums.push_back(num);

            // Continue search starting from the next character from the digit
            if (iNumEnd+1 < line.size())
            {
                iNumPos = line.find_first_of(digits.c_str(), iNumEnd + 1);
            }
            else
            {
                iNumPos = line.npos;
            }
        }
    }

    return gearNums;
}

std::vector<int> ParseGearRatio(const string& prevLine, const string& curLine, const string& nextLine){
    
    
    //cout << "Line[i-1]: " << prevLine << endl;
    //cout << "Line[ i ]: " << curLine << endl;
    //cout << "Line[i+1]: " << nextLine << endl;

    string line = curLine;
    std::vector<int> gearRatios;

    size_t iPos = curLine.find('*', 0);
    while (iPos < curLine.size())
    {
        std::vector<int> gearNums;
        // We can have max 6 matches
        gearNums.reserve(6);

        size_t iStart = iPos > 0 ? iPos - 1 : 0;
        size_t iEnd = iPos + 1 < curLine.size() ? iPos + 1 : curLine.size() - 1;

        // Extract from prevLine
        {
            std::vector<int> newGearNums = ExtractGearNums(iStart, iEnd, prevLine);
            gearNums.insert(gearNums.end(), newGearNums.begin(), newGearNums.end());
        }

        // Extract from left
        {
            std::vector<int> newGearNums = ExtractGearNums(iStart, iStart, curLine);
            gearNums.insert(gearNums.end(), newGearNums.begin(), newGearNums.end());
        }

        // Extract from right
        {
            std::vector<int> newGearNums = ExtractGearNums(iEnd, iEnd, curLine);
            gearNums.insert(gearNums.end(), newGearNums.begin(), newGearNums.end());
        }

        // Extract from nextLine
        {
            std::vector<int> newGearNums = ExtractGearNums(iStart, iEnd, nextLine);
            gearNums.insert(gearNums.end(), newGearNums.begin(), newGearNums.end());
        }

        // Check if we have exactly 2 matches
        if( gearNums.size()==2 )
        {
            gearRatios.push_back( gearNums[0]*gearNums[1] );
        }

        // Find the next gear symbol in the line
        if(iPos+1<curLine.size()){
            iPos = curLine.find('*', iPos+1);
        }
    }

    return gearRatios;
}

int main(){
    string line;
    getline(cin, line);
    //cout << "numLines: " << line << endl;

    int nLines = stoi(line);

    string prevLine, nextLine;

    
    getline(cin, line);

    std::vector<int> gearRatios;

    for( int idx = 0 ; idx < nLines-1; idx++ ){
        
        getline(cin, nextLine);


        std::vector<int> newRatios = ParseGearRatio(prevLine, line, nextLine);
        gearRatios.insert(gearRatios.end(), newRatios.begin(), newRatios.end());

        // shift lines
        prevLine.swap(line);
        line.swap(nextLine);
    }

    {
        nextLine = "";
        std::vector<int> newRatios = ParseGearRatio(prevLine, line, nextLine);
        gearRatios.insert(gearRatios.end(), newRatios.begin(), newRatios.end());
    
    }

    int sum = 0;
    for( const auto& n : gearRatios  ){
        sum += n;
    }

    cout << "Answer: "<<sum<<endl;

    return 0;
}