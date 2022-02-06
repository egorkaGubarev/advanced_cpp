#include <iostream>
#include <chrono>
#include <cmath>
#include <string>

typedef unsigned int uint;

template <typename format>
class Timer{
    public:
        Timer(): start_{std::chrono::steady_clock::now()}, delta_{0}, status_{"pause"}{}

        void run()
        {
            if (status_ == "pause") {
                start_ = std::chrono::steady_clock::now();
                status_ = "run";
            }
        }

        void pause()
        {
            if (status_ == "run") {
                const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                const std::chrono::duration delta = now - start_;
                status_ = "pause";
                delta_ += delta;
            }
        }

        ~Timer()
        {
            pause();
            const auto interval = std::chrono::duration_cast<format>(delta_);
            const long long interval_delta = interval.count();
            std::cout << "Time: " << interval_delta << '\n';
        }

    private:
        std::string status_;
        std::chrono::steady_clock::time_point start_;
        std::chrono::steady_clock::duration delta_;
};

double calculate(double start, uint cycles)
{
    double result = start;
    for(uint i = 0; i < cycles; ++ i){
        result += std::sin(i) + std::cos(i);
    }
    return result;
}

int main()
{
    const uint cycles = 100000;
    double result = 0;
    {
        Timer<std::chrono::milliseconds> my_timer;
        my_timer.run();
        result = calculate(result, cycles);
        my_timer.pause();
        result = calculate(result, cycles);
        my_timer.run();
        result = calculate(result, cycles);
        my_timer.pause();
    }
    std::cout << "Result: " << result;
    return 0;
}
