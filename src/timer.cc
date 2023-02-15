#include <chrono>

class Timer {
    public:
        Timer() : _start(std::chrono::system_clock::now()) {}
        
        void reset(){
            _start = std::chrono::system_clock::now();
        }

        float seconds(){
            const auto end = std::chrono::system_clock::now();
            std::chrono::duration<float> dt = end - _start;
            return dt.count();
        }

    public:
    std::chrono::time_point<std::chrono::system_clock> _start;
};
