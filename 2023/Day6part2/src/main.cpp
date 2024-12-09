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

    string timeStr = line.substr(idxStart);
    auto itr = std::find(timeStr.begin(), timeStr.end(), ' ');
    while( itr != timeStr.end())
    {
        timeStr.erase(itr);
        itr = std::find(timeStr.begin(), timeStr.end(), ' ');
    }
    cout << "Time: " << timeStr << endl;
    size_t time = stoull(timeStr);

    getline(cin, line);
    cout << line << endl;
    idxStart = line.find(':')+1;
    string distanceStr = line.substr(idxStart);
    itr = std::find(distanceStr.begin(), distanceStr.end(), ' ');
    while( itr != distanceStr.end())
    {
        distanceStr.erase(itr);
        itr = std::find(distanceStr.begin(), distanceStr.end(), ' ');
    }
    cout << "Distance: " << distanceStr << endl;
    size_t distance = stoull(distanceStr);
    Race race(time, distance);
    size_t numWays = race.CountNumberOfWaysToWin();


    cout << "Result: " << numWays << endl;

    return 0;
}