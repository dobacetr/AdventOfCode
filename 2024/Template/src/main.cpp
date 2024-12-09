#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

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
    std::vector<int> left;
    std::vector<int> right;

    // Parse columns into vectors
    Utility::Parse(*inStream, left, right);

    // Print vectors as side-by-side columns
    Utility::Print(std::cout, left, right);

    return 0;
}