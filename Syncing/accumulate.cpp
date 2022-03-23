#include <algorithm>
#include <atomic>
#include <iostream>
#include <numeric>
#include <vector>
#include <thread>

typedef unsigned int uint;

template <typename Iterator, typename T>
void accumulate_block(Iterator first, Iterator last, std::atomic<T>& init)
{
    while(first != last){
        const T value = *first;
        init += value;
        first = std::next(first);
    }
}

template <typename Iterator, typename T>
void parallel_accumulate(Iterator first, Iterator last, std::atomic<T>& init, uint num_workers)
{
    auto length = std::distance(first, last);
    if (length < 100'000){
        accumulate_block(first, last, init);
    }
    else{
        uint length_per_thread = (length + num_workers - 1) / num_workers;
        std::vector<std::thread> threads;
        for(uint i = 0; i < num_workers - 1; i ++){
            auto beginning = std::next(first, i * length_per_thread);
            auto ending = std::next(first, (i + 1) * length_per_thread);
            threads.push_back(std::thread(accumulate_block<Iterator, T>, beginning, ending, std::ref(init)));
        }
        accumulate_block(std::next(first, (num_workers - 1) * length_per_thread), last, init);
        std::for_each(std::begin(threads), std::end(threads), std::mem_fun_ref(&std::thread::join));
    }
}

int main()
{
    const uint element = 1;
    const uint size = 1'000'000;
    const uint workers = 4;
    std::atomic<uint> init = 1;
    std::vector<uint> test_sequence(size);
    std::fill(std::begin(test_sequence), std::end(test_sequence), element);
    parallel_accumulate(std::begin(test_sequence), std::end(test_sequence), init, workers);
    std::cout << init;
    return 0;
}
