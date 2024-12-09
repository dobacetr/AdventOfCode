#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

std::vector<int> ParseSplit(const string& str, const char delim){
    std::vector<int> numbers;

    size_t iPot = str.find_first_not_of(delim);
    size_t idx = iPot-1;
    while(idx<str.size()){

        size_t idxNext = str.find(delim, idx+1);

        // Number is from idx+1 to idxNext-1
        string numStr = str.substr(idx+1, idxNext-(idx+1));

        //cout << "Number is: " << numStr << endl;

        int num = stoi(numStr);

        numbers.push_back(num);

        // Find next delimiter
        size_t iPot = str.find_first_not_of(delim, idxNext);
        idx = iPot-1;
        
    }

    return numbers;
}

void ParseWinningAndHand(const string& line, std::vector<int>& outWinning, std::vector<int>& outHand){
    size_t idxWinStart = line.find(':') + 1;
    size_t idxWinEnd = line.find('|')-1;
    size_t idxHandStart = idxWinEnd+2;
    size_t idxHandEnd = line.size()-1;

    outWinning  = ParseSplit(line.substr(idxWinStart    , idxWinEnd-idxWinStart+1   ), ' ');
    outHand     = ParseSplit(line.substr(idxHandStart   , idxHandEnd-idxHandStart+1 ), ' ');
}

int CountMatches(const std::vector<int>& in1, const std::vector<int>& in2){
    int countMatch = 0;
    for( const int num : in1 ){
        auto itr = std::find( in2.begin(), in2.end(), num );
        if( itr != in2.end() ){
            countMatch++;
        }
    }
    return countMatch;
}

int main(){
    
    string numLinesStr;
    getline(cin, numLinesStr);
    int nLines = stoi(numLinesStr);

    std::cout << "#Lines: " << nLines << endl;

    std::vector<size_t> numCopies;
    numCopies.resize(nLines);

    for( int iLine = 0; iLine < nLines; iLine++ ){
        string line;
        getline(cin, line);
        std::cout <<"Line: " << line << endl;

        std::vector<int> winningNumbers, handNumbers;
        ParseWinningAndHand(line, winningNumbers, handNumbers);

        //cout << "Winning numbers are: ";
        //for( const int winNum : winningNumbers ){
        //    cout << winNum << " ";
        //}
        //cout << endl;
//
        //cout << "Hand numbers are: ";
        //for( const int handNum : handNumbers ){
        //    cout << handNum << " ";
        //}
        //cout << endl;

        int countMatch = CountMatches(winningNumbers, handNumbers);

        //std::cout<< "Number of matches:" << countMatch << endl;
        //std::cout<< "Number of copies:" << numCopies[iLine]+1 << endl;

        for( int idx = 0; idx < countMatch; idx++ ){
            numCopies[idx+iLine+1]+=numCopies[iLine]+1;
        }
    }

    int totalCards = nLines;
    for( const auto num : numCopies ){
        totalCards += num;
    }    

    std::cout << "Total cards: " << totalCards << endl;

    return 0;
}