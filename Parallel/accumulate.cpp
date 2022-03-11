#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <string>
#include <thread>

typedef unsigned int uint;

template <typename Iterator, typename T>
void accumulate_block(Iterator first, Iterator last, T init, T& result)
{
    result = std::accumulate(first, last, init);
}

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init, uint num_workers)
{
    auto length = std::distance(first, last);
    if (length < 100'000){
        return std::accumulate(first, last, init);
    }
    auto length_per_thread = (length + num_workers - 1) / num_workers;
    std::vector<std::thread> threads;
    std::vector<T> results(num_workers - 1);
    for(auto i = 0u; i < num_workers - 1; i ++){
        auto beginning = std::next(first, i * length_per_thread);
        auto ending = std::next(first, (i + 1) * length_per_thread);
        threads.push_back(std::thread(accumulate_block<Iterator, T>, beginning, ending, 0, std::ref(results[i])));
    }
    auto main_result = std::accumulate(std::next(first, (num_workers - 1) * length_per_thread), last, init);
    std::for_each(std::begin(threads), std::end(threads), std::mem_fun_ref(&std::thread::join));
    return std::accumulate(std::begin(results), std::end(results), main_result);
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

    ~Timer()
    {
        pause();
        const long long time = get_time();
        std::cout << "Time: " << time << '\n';
    }

private:
    bool is_running_;
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::duration delta_;
};

int main()
{
    const uint workers_min = 1;
    const uint workers_max = 8;
    const uint size = 1'000'000;
    const std::string path{"result.txt"};
    std::ofstream out;
    std::vector<int> test_sequence(size);
    std::iota(std::begin(test_sequence), std::end(test_sequence), 0);
    out.open(path);
    for(uint workers = workers_min; workers <= workers_max; ++ workers){
        Timer<std::chrono::microseconds> timer;
        timer.run();
        auto result = parallel_accumulate(std::begin(test_sequence), std::end(test_sequence), 0,
                                          workers);
        timer.pause();
        const uint time = timer.get_time();
        std::cout << result << std::endl;
        out << workers << "; " << time << '\n';
    }
    out.close();
    return 0;
}
