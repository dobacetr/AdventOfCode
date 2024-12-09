#include <iostream>
#include <string>
#include <algorithm>

bool isADigit(const uint8_t c){
    return c >= '0' && c <= '9';
}

int main(){
    int numLines;
    std::cin >> numLines;
    int sum = 0;
    for( int i = 0; i < numLines; i++ ){
        std::string line;
        std::cin >> line;

        auto first = std::find_if(line.begin(), line.end(), isADigit);
        auto last = std::find_if(line.rbegin(), line.rend(), isADigit);

        const char one = *first;
        const char two = *last;

        std::string val;
        val += one;
        val += two;

        std::cout << val << std::endl;
        int ival = std::stoi(val);
        sum += ival;
    }

    std::cout << sum;

    return 0;
}