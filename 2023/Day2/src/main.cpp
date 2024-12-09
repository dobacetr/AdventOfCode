#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

struct CubeCounts{
    int red = 0;
    int green = 0;
    int blue = 0;
};

void SplitStr(string& in_outAfter, string& outBefore, const char sep){
    auto idxEnd = in_outAfter.find(sep);
    outBefore = in_outAfter.substr(0, idxEnd);
    if( idxEnd == in_outAfter.npos ){
        in_outAfter = "";
        return;
    }
    in_outAfter = in_outAfter.substr(idxEnd+1);
}

int ConsumeGameId(string& line){
    constexpr int gameIdStartP = 5;
    const int gameIdEndP = line.find(':');
    string gameIdStr = line.substr(gameIdStartP, gameIdEndP-(gameIdStartP));
    int gameId = stoi(gameIdStr);

    line = line.substr(gameIdEndP+1);
    return gameId;
}

void ConsumeCubeType(string& line, CubeCounts& counts){
    string cubeStr;
    SplitStr(line, cubeStr, ',');

    cubeStr = cubeStr.substr( cubeStr.find_first_not_of(' '));

    auto idxSpace = cubeStr.find(' ');
    string numstr = cubeStr.substr(0, idxSpace);
    int num = stoi(numstr);
    
    if( cubeStr.find("red") != cubeStr.npos){
        counts.red += num;
        return;
    } 
    
    if( cubeStr.find("green") != cubeStr.npos){
        counts.green += num;
        return;
    } 
    
    if( cubeStr.find("blue") != cubeStr.npos){
        counts.blue += num;
        return;
    }

    cout << "Couldn't find what we had: " << cubeStr << endl;
}

CubeCounts ConsumeCubeCounts(string& line){
    CubeCounts result;
    string subsetStr;
    SplitStr(line, subsetStr, ';');

    while(subsetStr.size()>0){
        ConsumeCubeType(subsetStr, result);
    }

    return result;
}

bool IsRoundPossible(const CubeCounts& game, const CubeCounts& limits){
    //cout << "red   :" << game.red << "\t<=" << limits.red << "\t?" << endl;
    //cout << "blue  :" << game.blue << "\t<=" << limits.blue << "\t?" << endl;
    //cout << "green :" << game.green << "\t<=" << limits.green << "\t?" << endl;
    return game.red <= limits.red &&
    game.blue <= limits.blue &&
    game.green <= limits.green;
}



int main(){
    std::string nLS;
    std::getline(std::cin, nLS);
    int numLines = stoi(nLS);

    int sum = 0;

    CubeCounts limits;
    limits.red = 12;
    limits.green = 13;
    limits.blue = 14;

    for( int i = 0; i < numLines; i++ ){
        std::string line;
        std::getline(std::cin, line);
        cout << line << endl;
        const int gameId = ConsumeGameId(line);
        bool possible = true;
        while(line.size()>0 && possible){
            CubeCounts roundResult = ConsumeCubeCounts(line);
            possible = possible && IsRoundPossible(roundResult, limits);
        }
        if(possible){
            sum+=gameId;
        }
    }

    std::cout << sum;

    return 0;
}