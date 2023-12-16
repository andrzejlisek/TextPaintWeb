#include "stopwatch.h"

Stopwatch::Stopwatch()
{

}

void Stopwatch::Reset()
{
    Time1 = std::chrono::high_resolution_clock::now();
}

int Stopwatch::Elapsed()
{
    Time2 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration Duration = Time2 - Time1;
    return std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count();
}

void Stopwatch::MeasureTick()
{
    TickPeriod = -1;
    CallibrationI = 10;
}

void Stopwatch::Tick()
{
    if (CallibrationI != 0)
    {
        CallibrationI--;
        if ((CallibrationI & 1) != 0)
        {
            Tick2 = std::chrono::high_resolution_clock::now();
            Callibration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(Tick2 - Tick1).count());
        }
        else
        {
            Tick1 = std::chrono::high_resolution_clock::now();
            Callibration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(Tick1 - Tick2).count());
        }
        if (CallibrationI == 0)
        {
            std::sort(Callibration.begin() + 1, Callibration.end());
            int I = 5;
            TickPeriod = (Callibration[I - 1] + Callibration[I] + Callibration[I + 1]) / 3;
        }
    }
}
