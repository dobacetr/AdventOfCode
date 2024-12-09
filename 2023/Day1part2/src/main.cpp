#include <iostream>
#include <string>
#include <algorithm>
#include <array>

void ParseFirstAndLastSpelledDigits(std::string& string){

    const std::array<std::string, 9> numbers{
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };
    const std::array<std::string, 9> digits{
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };
    for( int i = 0; i < numbers.size(); i++ ){
        {
            auto idx = string.find(numbers[i]);
            if( idx != string.npos ){
                string.replace(idx+1, numbers[i].size()-2, digits[i]);
            }
        }
        {
            auto idx = string.rfind(numbers[i]);
            if( idx != string.npos ){
                string.replace(idx+1, numbers[i].size()-2, digits[i]);
            }
        }
    }
}

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

        ParseFirstAndLastSpelledDigits(line);

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