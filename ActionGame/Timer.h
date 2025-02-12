#pragma once
#include <windows.h>


class Timer {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER previousTime;
public:
    Timer() {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&previousTime);
    }
    float GetDeltaTime();
};
