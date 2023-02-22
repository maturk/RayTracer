#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include "common.h"

RAYTRACER_NAMESPACE_BEGIN

class Timer {
    public:
        Timer();
        void reset();
        float end();

    public:
    std::chrono::time_point<std::chrono::system_clock> _start;
};

RAYTRACER_NAMESPACE_END
#endif