#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <forward_list>
#include <random>
#include <vector>

typedef unsigned int uint;

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

class UniformRandomGenerator
{
public:
    explicit UniformRandomGenerator(long long seed): seed_{seed}
    {
        if (seed == -1){
            seed_ = time(nullptr);
        }
        engine_.seed(seed_);
    }

    std::vector<int> create_sequence(const unsigned int n, const int min, const int max)
    {
        std::uniform_int_distribution<int> distr(min, max);
        std::vector<int> sequence(n);
        for (unsigned int i = 0; i < n; ++ i){
            sequence[i] = distr(engine_);
        }
        return sequence;
    }

private:
    long long seed_;
    std::mt19937_64 engine_;
};

template <typename container, typename type>
void fill(container& cont, UniformRandomGenerator& generator)
{
    const uint min = 0;
    const uint max = 1'000'000'000;
    for(type& value: cont){
        const std::vector<int> random = generator.create_sequence(1, min, max);
        const uint number = random[0];
        value = number;
    }
}

template <typename container>
void sort(UniformRandomGenerator& generator)
{
    const uint size_max = 1'000'000;
    uint size = 5;
    const float step = 1.1;
    const std::string path = "result.txt";
    std::ofstream out;
    out.open(path);
    while(size < size_max){
        container cont(size);
        fill<container, uint>(cont, generator);
        Timer<std::chrono::milliseconds> timer;
        timer.run();
        auto end = std::end(cont);
        std::sort(std::begin(cont), end);
        timer.pause();
        const long long time = timer.get_time();
        out << size << "; " << time << '\n';
        const float size_float = (float)size * step;
        size = std::ceil(size_float);
    }
    out.close();
}

template <uint size>
void sort_array(UniformRandomGenerator& generator)
{
    std::array<uint, size> cont{};
    fill<std::array<uint, size>, uint>(cont, generator);
    Timer<std::chrono::milliseconds> timer;
    timer.run();
    auto end = std::end(cont);
    std::sort(std::begin(cont), end);
    timer.pause();
}

template <typename container>
void sort_method(UniformRandomGenerator& generator)
{
    const uint size_max = 1'000'000;
    uint size = 5;
    const float step = 1.1;
    const std::string path = "result.txt";
    std::ofstream out;
    out.open(path);
    while(size < size_max){
        container cont(size);
        fill<container, uint>(cont, generator);
        Timer<std::chrono::milliseconds> timer;
        timer.run();
        cont.sort();
        timer.pause();
        const long long time = timer.get_time();
        out << size << "; " << time << '\n';
        const float size_float = (float)size * step;
        size = std::ceil(size_float);
    }
    out.close();
}

int main()
{
    const uint seed = 0;
    UniformRandomGenerator generator(seed);
    sort_method<std::forward_list<uint>>(generator);
    return 0;
}
