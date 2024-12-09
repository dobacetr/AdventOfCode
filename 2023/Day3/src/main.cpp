#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

const char emptyChar = '.';
const string digits = "0123456789";

bool IsSpecialCharacter(const char c){
    bool notDigit = digits.find(c) == digits.npos;
    bool notEmpty = c != emptyChar;
    return notDigit && notEmpty;
    
}

std::vector<int> ParsePartNumbers(const string& prevLine, const string& curLine, const string& nextLine){
    string line = curLine;
    std::vector<int> result;
    cout << curLine << endl; 

    size_t iStart = curLine.find_first_of(digits, 0);
    while( iStart < curLine.size()){
        size_t iEnd = curLine.find_first_not_of(digits, iStart);
        string numStr = curLine.substr(iStart, iEnd-iStart);
        int num = stoi(numStr);

        //cout << num << endl;

        // we have num
        // its column starts at iStart, ends at iEnd
        // check prevLine iStart-1 to iEnd+1
        // Check curLine iStart-1 and iEnd+1
        // check nextLine iStart-1 to iEnd+1
        int is = iStart > 0 ? iStart -1 : 0;
        
        bool containsSymbol = false;

        if(prevLine.size()>0){
            string str = prevLine.substr(is, iEnd+1-is);
            bool result =  str.end() != find_if(str.begin(), str.end(), IsSpecialCharacter);
            containsSymbol = containsSymbol || result;
               
            cout << "Checking Up: " << str << " -> " << result<< endl;
        }

        if(iStart>0){
            bool result =  IsSpecialCharacter(curLine[iStart-1]);
            containsSymbol = containsSymbol || result;
            cout << "Checking Left: " << curLine[iStart-1] << " -> " << result<< endl;
        }

        if(iEnd<curLine.size()){
            bool result =  IsSpecialCharacter(curLine[iEnd]);
            containsSymbol = containsSymbol || result;
            cout << "Checking Right: " << curLine[iEnd] << " -> " << result<< endl;
        }

        if(nextLine.size()>0){
            string str = nextLine.substr(is, iEnd+1-is);
            bool result =  str.end() != find_if(str.begin(), str.end(), IsSpecialCharacter);
            containsSymbol = containsSymbol || result;
            cout << "Checking Down: " << str << " -> " << result<< endl;
        }

        cout << "Result for " << num << " is " << containsSymbol << endl;
        if(containsSymbol){
            result.push_back(num);
        }

        if( iEnd < curLine.size()-1 ){
            iStart = curLine.find_first_of(digits, iEnd+1);
        }
        else{
            iStart = curLine.size();
        }
    }

    return result;
}

int main(){
    string line;
    getline(cin, line);
    cout << "numLines: " << line << endl;

    int nLines = stoi(line);

    string prevLine, nextLine;

    
    getline(cin, line);
    //prevLine.reserve(line.length());
    //std::fill(prevLine.begin(), prevLine.end(), emptyChar);

    std::vector<int> partNumbers;

    for( int idx = 0 ; idx < nLines-1; idx++ ){
        
        getline(cin, nextLine);


        cout << "Line" << idx+1 << ": " ;
        std::vector<int> newNumbers = ParsePartNumbers(prevLine, line, nextLine);
        partNumbers.insert(partNumbers.end(), newNumbers.begin(), newNumbers.end());

        // shift lines
        prevLine.swap(line);
        line.swap(nextLine);
    }

    {
        nextLine = "";
        cout << "Line" << nLines << ": " ;
        std::vector<int> newNumbers = ParsePartNumbers(prevLine, line, nextLine);
        partNumbers.insert(partNumbers.end(), newNumbers.begin(), newNumbers.end());
    
    }

    int sum = 0;
    for( const auto& n : partNumbers  ){
        sum += n;
    }

    cout << "Answer: "<<sum<<endl;

    return 0;
}