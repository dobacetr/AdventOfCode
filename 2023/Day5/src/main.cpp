#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct SymbolicMap{
    size_t destBegin;
    size_t sourceBegin;
    size_t sourceEnd;//Inclusive

    SymbolicMap(const size_t destBegin,
        const size_t sourceBegin,
        const size_t sourceEnd
    ) : destBegin(destBegin), sourceBegin(sourceBegin), sourceEnd(sourceEnd) {}

    bool Contains(const size_t idx) const{
        return idx >= sourceBegin &&
        idx<=sourceEnd;
    }

    size_t operator()(const size_t idx) const{
        return (idx-sourceBegin)+destBegin;
    }
};

class SymbolicMapper
{
public:
    std::vector<SymbolicMap> maps;

    void AddMap(const SymbolicMap& newMap){
        maps.push_back(newMap);
    }

    size_t operator()(const size_t idx){
        for( const SymbolicMap& map: maps ){
            if( map.Contains(idx) )
            {
                return map(idx);
            }
        }
        
        // return input if no symbolic mapping was done
        return idx;
    }
};

std::vector<size_t> ParseNumbers(const string& line, const size_t pos=0){
    
    std::vector<size_t> numbers;

    size_t idxStart = line.find_first_not_of(' ', pos);
    while(idxStart<line.size()){
        size_t idxEnd = line.find(' ', idxStart);
        string numStr = line.substr(idxStart, idxEnd-idxStart);

        //cout << "Number: " << numStr << endl;

        size_t num = stoull(numStr);

        numbers.push_back(num);
        idxStart = line.find_first_not_of(' ', idxEnd);
    }

    return numbers;
}

std::vector<size_t> ParseSeeds(const string& line){
    size_t idxStart = line.find(':') + 1;

    std::vector<size_t> seeds = ParseNumbers(line, idxStart);

    return seeds;
}

void ParseMap(const string& headerString, SymbolicMapper& inOutMapper, string& line){
    
    // Skip empties
    while(line.size()==0){
        getline(cin, line);
    }

    if( line != headerString ){
        cout << "Expected: \"" << headerString <<"\" got \"" << line << "\"" << endl;
    }

    // Get maps

    // Get seed 2 soil map
    getline(cin, line);
    while( line.size()!=0 ){
        // We have a map
        std::vector<size_t> numbers = ParseNumbers(line);
        inOutMapper.AddMap( SymbolicMap(numbers[0], numbers[1], numbers[1]+numbers[2]-1) );
        getline(cin, line);
    }
}

int main(){
    
    // Read Seeds
    string line;
    getline(cin, line);
    
    std::vector<size_t> seeds = ParseSeeds(line);
    std::vector<size_t> locs;
    locs.reserve(seeds.size());

    SymbolicMapper seed2soil;
    SymbolicMapper soil2fert;
    SymbolicMapper fert2water;
    SymbolicMapper water2light;
    SymbolicMapper light2temp;
    SymbolicMapper temp2hum;
    SymbolicMapper hum2loc;

    // Skip empties
    getline(cin, line);
    ParseMap("seed-to-soil map:"            , seed2soil     , line);
    ParseMap("soil-to-fertilizer map:"      , soil2fert     , line);
    ParseMap("fertilizer-to-water map:"     , fert2water    , line);
    ParseMap("water-to-light map:"          , water2light   , line);
    ParseMap("light-to-temperature map:"    , light2temp    , line);
    ParseMap("temperature-to-humidity map:" , temp2hum      , line);
    ParseMap("humidity-to-location map:"    , hum2loc       , line);

    for( const size_t seed : seeds ){
        const size_t soil = seed2soil(seed);
        const size_t fert = soil2fert(soil);
        const size_t water = fert2water(fert);
        const size_t light = water2light(water);
        const size_t temp = light2temp(light);
        const size_t hum = temp2hum(temp);
        const size_t loc = hum2loc(hum);
        locs.push_back(loc);
    }

    size_t firstLoc = *std::min_element(locs.begin(), locs.end());

    cout << "First Location is: " << firstLoc << endl;

    return 0;
}