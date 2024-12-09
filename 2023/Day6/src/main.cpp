#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

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

struct Race
{
    size_t duration;
    size_t record;

    Race(const size_t duration, const size_t record) :
    duration(duration), record(record) {}

    size_t CountNumberOfWaysToWin() const{
        const double acc = 1.0;
        // vel = t*acc
        // dist = vel * (duration-t)

        // record/acc = t  * duration - t*t
        // t^2 - duration*t + record/acc = 0

        double a = 1.0;
        double b = -static_cast<double>(duration);
        double c = static_cast<double>(record)/acc;

        // del = sqrt(b^2-4ac)

        double del = sqrt( b*b - 4 * a * c );


        double t1 = (- b - del)/(2*a);
        double t2 = (- b + del)/(2*a);

        cout << "t1: " << t1 << " t2:" << t2 << endl;

        size_t numWays = std::floor(t2*(1.0-1E-15)) - std::ceil(t1*(1.0+1E-15)) + 1;
        return numWays;

    }
};

int main(){
    
    string line;
    getline(cin, line);
    
    cout << line << endl;
    size_t idxStart = line.find(':')+1;

    std::vector<size_t> times = ParseNumbers(line, idxStart);

    getline(cin, line);
    cout << line << endl;
    idxStart = line.find(':')+1;
    std::vector<size_t> distances = ParseNumbers(line, idxStart);

    std::vector<Race> races;
    races.reserve(times.size());

    size_t result = 1;

    for( size_t idx = 0; idx < times.size(); idx++ )
    {
        Race race(times[idx], distances[idx]);
        races.push_back(race);

        size_t numWays = race.CountNumberOfWaysToWin();
        cout << "Race " << idx << ": " << numWays << endl;

        result *= numWays;
    }

    cout << "Result: " << result << endl;

    return 0;
}