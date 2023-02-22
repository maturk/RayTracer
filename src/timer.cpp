#include "timer.h"

RAYTRACER_NAMESPACE_BEGIN

    Timer::Timer() : _start(std::chrono::system_clock::now()) {}

    void Timer::reset() {
        _start = std::chrono::system_clock::now();
    }

    float Timer::end(){
        const auto end = std::chrono::system_clock::now();
        std::chrono::duration<float> dt = end - _start;
        return dt.count();
    }

RAYTRACER_NAMESPACE_END