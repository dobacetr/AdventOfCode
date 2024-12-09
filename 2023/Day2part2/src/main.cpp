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

void MinimizeCubeCounts(const CubeCounts& game, CubeCounts& prevMin){
    if(prevMin.red<game.red){
        prevMin.red = game.red;
    }
    if(prevMin.blue<game.blue){
        prevMin.blue = game.blue;
    }
    if(prevMin.green<game.green){
        prevMin.green = game.green;
    }
}

int ComputePowerOfGame(const CubeCounts& game){
    return game.red*game.green*game.blue;
}

int main(){
    std::string nLS;
    std::getline(std::cin, nLS);
    int numLines = stoi(nLS);

    int sum = 0;

    for( int i = 0; i < numLines; i++ ){
        CubeCounts minCounts;
        std::string line;
        std::getline(std::cin, line);
        cout << line << endl;
        const int gameId = ConsumeGameId(line);
        while(line.size()>0){
            CubeCounts roundResult = ConsumeCubeCounts(line);
            MinimizeCubeCounts(roundResult, minCounts);
        }
        int powGame = ComputePowerOfGame(minCounts);
        cout << powGame << endl;
        sum+=powGame;
    }

    std::cout << sum;

    return 0;
}