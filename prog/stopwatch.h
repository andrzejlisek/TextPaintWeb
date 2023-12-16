#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <algorithm>
#include <vector>

class Stopwatch
{
public:
    Stopwatch();
    void Reset();
    int Elapsed();
    void Tick();
    void MeasureTick();
    int TickPeriod = -1;
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> Time1;
    std::chrono::time_point<std::chrono::high_resolution_clock> Time2;
    std::chrono::time_point<std::chrono::high_resolution_clock> Tick1;
    std::chrono::time_point<std::chrono::high_resolution_clock> Tick2;
    std::vector<int> Callibration;
    int CallibrationI = 0;
};

#endif // STOPWATCH_H
