#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

struct Range{
    size_t rangeBegin;
    size_t rangeEnd;

    Range(const size_t rangeBegin, const size_t rangeEnd) :
        rangeBegin(rangeBegin), rangeEnd(rangeEnd) {}
};

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

        std::vector<SymbolicMap>::iterator itr =
            std::find_if( maps.begin(), maps.end(),
            // map whose begin is bigger than us
                [newMap](const SymbolicMap& map)->bool{
                    return map.sourceBegin>newMap.sourceBegin;
        } );

        maps.insert(itr, newMap);
    }

    size_t operator()(const size_t idx) const{
        for( const SymbolicMap& map: maps ){
            if( map.Contains(idx) )
            {
                return map(idx);
            }
        }
        
        // return input if no symbolic mapping was done
        return idx;
    }

    size_t FindContainingMapIdx( const size_t idx, const size_t pos=0 ) const{
        std::vector<SymbolicMap>::const_iterator itr =
            std::find_if(maps.begin()+pos, maps.end(),
                // Containing map
                [idx](const SymbolicMap& map)->bool{
                    return map.Contains(idx);
        });

        size_t result = itr - maps.begin();
        return result;
    }

    size_t FindFirstMapInRange(const Range& range) const{
        return maps.size();
    }

    std::vector<Range> operator()(const Range inRange) const{
        Range range = inRange;
        std::vector<Range> destinationRange;

        // Find the map idx that contains begin of the range
        size_t idxMap = FindContainingMapIdx(range.rangeBegin);

        // We rangeBegin does not belong to a map
        if( idxMap == maps.size() ){
            // We need to find the first map that is contained within the range
            idxMap = FindFirstMapInRange(inRange);

            if( idxMap == maps.size() ){
                // No map within range, return range as it is
                destinationRange.push_back(inRange);
                return destinationRange;
            }

            // split range from beginning
            Range newRange(range.rangeBegin, maps[idxMap].sourceBegin-1);
            range.rangeBegin = maps[idxMap].sourceBegin;

            // Get the transform range of the other part of the range
            std::vector<Range> newDestinationRange = operator()(newRange);

            // Combine destinations ranges
            destinationRange.insert(destinationRange.end(), newDestinationRange.begin(), newDestinationRange.end());
        }

        // Find the fracture point which spawns a new range
        size_t mapLastSource = maps[idxMap].sourceEnd;

        if( range.rangeEnd>mapLastSource ){
            // Split the input range
            Range newRange(mapLastSource+1, range.rangeEnd);
            range.rangeEnd = mapLastSource;

            // Get the transform range of the other part of the range
            std::vector<Range> newDestinationRange = operator()(newRange);

            // Combine destinations ranges
            destinationRange.insert(destinationRange.end(), newDestinationRange.begin(), newDestinationRange.end());

        }

        Range destRange( maps[idxMap].operator()(range.rangeBegin),
                         maps[idxMap].operator()(range.rangeEnd)  );
        destinationRange.push_back( destRange );

        return destinationRange;
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
    getline(cin, line);
    while( line.size()!=0 ){
        // We have a map
        std::vector<size_t> numbers = ParseNumbers(line);
        inOutMapper.AddMap( SymbolicMap(numbers[0], numbers[1], numbers[1]+numbers[2]-1) );
        getline(cin, line);
    }
}

std::vector<Range> RangeTransform( const SymbolicMapper& inMapper, const std::vector<Range> inRanges ){
    std::vector<Range> outRanges;
    for( const Range inRange : inRanges ){
        std::vector<Range> newRange = inMapper(inRange);
        outRanges.insert(outRanges.end(), newRange.begin(), newRange.end());
    }
    return outRanges;
}

void PrintRange(const Range& range){
    cout << range.rangeBegin << "-->" << range.rangeEnd << endl;
}

void PrintRanges(const std::vector<Range>& ranges){
    for( const Range& range: ranges ){
        PrintRange(range);
    }
}

int main(){
    
    // Read Seeds
    string line;
    getline(cin, line);
    
    std::vector<size_t> seeds = ParseSeeds(line);
    std::vector<size_t> locs;
    locs.reserve(seeds.size());

    std::array<SymbolicMapper, 7> mappers;

    SymbolicMapper& seed2soil   = mappers[0];
    SymbolicMapper& soil2fert   = mappers[1];
    SymbolicMapper& fert2water  = mappers[2];
    SymbolicMapper& water2light = mappers[3];
    SymbolicMapper& light2temp  = mappers[4];
    SymbolicMapper& temp2hum    = mappers[5];
    SymbolicMapper& hum2loc     = mappers[6];

    // Skip empties
    getline(cin, line);
    ParseMap("seed-to-soil map:"            , seed2soil     , line);
    ParseMap("soil-to-fertilizer map:"      , soil2fert     , line);
    ParseMap("fertilizer-to-water map:"     , fert2water    , line);
    ParseMap("water-to-light map:"          , water2light   , line);
    ParseMap("light-to-temperature map:"    , light2temp    , line);
    ParseMap("temperature-to-humidity map:" , temp2hum      , line);
    ParseMap("humidity-to-location map:"    , hum2loc       , line);

    std::vector<Range> inRanges;
    for( int idx = 0; idx < seeds.size(); idx+=2 ){
        const Range seedRange( seeds[idx], seeds[idx] + seeds[idx+1]-1 );
        PrintRange(seedRange);
        inRanges.push_back(seedRange);
    }

    cout << "Seed Ranges:" << endl;
    PrintRanges(inRanges);
    cout << endl;

    for( const SymbolicMapper& mapper : mappers )
    {
        inRanges = RangeTransform( mapper, inRanges );
        PrintRanges(inRanges);
        cout << endl;
    }

    std::vector<Range>::iterator itr= std::min_element(inRanges.begin(), inRanges.end(), [](const Range& left, const Range& right){
        return left.rangeBegin < right.rangeBegin;
    });

    size_t firstLoc = (*itr).rangeBegin;

    cout << "First Location is: " << firstLoc << endl;

    return 0;
}