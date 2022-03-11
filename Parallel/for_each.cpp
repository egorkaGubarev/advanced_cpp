#include <chrono>
#include <future>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

typedef unsigned int uint;

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& container)
{
    out << '[';
    std::copy(std::begin(container), std::prev(std::end(container)), std::ostream_iterator<T>(out, ", "));
    out << *(std::prev(std::end(container))) << ']';
    return out;
}

template <typename InputIt, typename UnaryFunction>
UnaryFunction for_each_in_block(InputIt first, InputIt last, UnaryFunction f)
{
    return std::for_each(first, last, f);
}

template <typename format>
class Timer{
public:
    Timer(): start_{std::chrono::steady_clock::now()}, delta_{0}, is_running_{false}{}

    void run()
    {
        if (!is_running_) {
            start_ = std::chrono::steady_clock::now();
            is_running_ = true;
        }
    }

    void pause()
    {
        if (is_running_) {
            const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            const std::chrono::duration delta = now - start_;
            is_running_ = false;
            delta_ += delta;
        }
    }

    long long get_time()
    {
        const auto interval = std::chrono::duration_cast<format>(delta_);
        const long long time = interval.count();
        return time;
    }

private:
    bool is_running_;
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::duration delta_;
};

template <typename InputIt, typename UnaryFunction>
UnaryFunction for_each_parallel(InputIt first, InputIt last, UnaryFunction f, const uint num_workers)
{
    const uint length_max = 100'000;
    const uint length = std::distance(first, last);
    if (length < length_max){
        return for_each_in_block(first, last, f);
    }
    const uint length_per_worker = (length + num_workers - 1) / num_workers;
    Timer<std::chrono::microseconds> external;
    external.run();
    for(uint i = 0; i < num_workers; ++ i){
        Timer<std::chrono::microseconds> internal;
        internal.run();
        InputIt beginning = std::next(first, i * length_per_worker);
        InputIt ending = std::next(first, (i + 1) * length_per_worker);
        std::future<UnaryFunction> my_future = std::async(std::launch::async, for_each_in_block<InputIt, UnaryFunction>,
                beginning, ending, f);
        internal.pause();
        const uint internal_time = internal.get_time();
        std::cout << "Internal: " << internal_time << " mcs" << '\n';
    }
    external.pause();
    const uint external_time = external.get_time();
    std::cout << "External: " << external_time << " mcs" << '\n';
    return f;
}

int main()
{
    const uint element_first = 0;
    const uint size = 100'000;
    const uint workers = 1;
    std::vector<uint> test(size);
    std::iota(std::begin(test), std::end(test), element_first);
    for_each_parallel(std::begin(test), std::end(test), [](const uint x){return x * x;},
                      workers);
    return 0;
}
