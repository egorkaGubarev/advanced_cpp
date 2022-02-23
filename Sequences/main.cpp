#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

typedef unsigned short int ushort;

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& container)
{
    out << '[';
    std::copy(std::begin(container), std::prev(std::end(container)), std::ostream_iterator<T>(out, ", "));
    out << *(std::prev(std::end(container))) << ']';
    return out;
}

bool is_prime(const int number)
{
    if(number == 1){
        return false;
    }
    auto root = (float)std::sqrt(number);
    const int max_divider = std::floor(root);
    int divider = 2;
    while(divider <= max_divider){
        const int mod = number % divider;
        if(mod == 0){
            return false;
        }
    }
    return true;
}

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

int main() {
    const ushort amount = 10, init = 1;
    std::vector<int> sequence_1(amount);
    auto begin = std::begin(sequence_1);
    auto end = std::end(sequence_1);
    std::iota(begin, end, init);
    std::cout << "Sequence 1: " << sequence_1 << '\n';
    std::cout << "How many numbers you want to input?" << '\n';
    ushort amount_input;
    std::cin >> amount_input;
    std::cout << "Enter " << amount_input << " numbers: " << '\n';
    for(ushort i = 0; i < amount_input; ++ i){
        int number;
        std::cin >> number;
        sequence_1.push_back(number);
    }
    std::cout << "Modified sequence 1: " << sequence_1 << '\n';
    std::random_device randomDevice;
    std::mt19937 get_random(randomDevice());
    std::shuffle(std::begin(sequence_1), std::end(sequence_1), get_random);
    std::cout << "Shuffled sequence 1: " << sequence_1 << '\n';
    std::sort(std::begin(sequence_1), std::end(sequence_1));
    auto non_unique_it = std::unique(std::begin(sequence_1), std::end(sequence_1));
    sequence_1.erase(non_unique_it, std::end(sequence_1));
    std::cout << "Unique sequence 1: " << sequence_1 << '\n';
    const ushort odd = std::count_if(std::begin(sequence_1), std::end(sequence_1),
                                     [](const int& number){return number % 2 == 1;});
    std::cout << "Odd: " << odd << '\n';
    const ushort min = sequence_1[0];
    const ushort size = sequence_1.size();
    const ushort max_index = size - 1;
    const ushort max = sequence_1[max_index];
    std::cout << "Min: " << min << "; max: " << max << '\n';
    auto prime_it = std::find_if(std::begin(sequence_1), std::end(sequence_1), is_prime);
    if(prime_it != std::end(sequence_1)){
        const int prime = *prime_it;
        std::cout << "Prime number: " << prime << '\n';
    }
    else{
        std::cout << "No prime numbers";
    }
    std::transform(std::begin(sequence_1), std::end(sequence_1), std::begin(sequence_1),
                   [](const int number){return std::pow(number, 2);});
    std::cout << "Squared sequence_1: " << sequence_1 << '\n';
    const long long seed = -1;
    UniformRandomGenerator generator(seed);
    const ushort min_random = 0;
    const ushort max_random = 100;
    std::vector<int> sequence_2 = generator.create_sequence(size, min_random, max_random);
    std::cout << "Sequence 2: " << sequence_2 << '\n';
    const int sum_init = 0;
    const ushort sum = std::accumulate(std::begin(sequence_2), std::end(sequence_2), sum_init);
    std::cout << "Sequence 2 sum: " << sum << '\n';
    const ushort amount_substitute = 3;
    const ushort substitution = 1;
    for(ushort i = 0; i < amount_substitute; ++ i){
        sequence_2[i] = substitution;
    }
    std::cout << "Substituted sequence 2: " << sequence_2 << '\n';
    std::vector<int> sequence_3(size);
    std::transform(std::begin(sequence_1), std::end(sequence_1), std::begin(sequence_2),
                   std::begin(sequence_3),
                   [](const int first, const int second){return first - second;});
    std::cout << "Sequence 3: " << sequence_3 << '\n';
    std::replace_if(std::begin(sequence_3), std::end(sequence_3),
                    [](const int number){return number < 0;}, 0);
    std::cout << "Non-negative sequence 3: " << sequence_3 << '\n';
    auto zero_it = std::remove_if(std::begin(sequence_3), std::end(sequence_3),
                                  [](const int number){return number ==0;});
    sequence_3.erase(zero_it, std::end(sequence_3));
    std::cout << "Non-zero sequence 3: " << sequence_3 << '\n';
    std::reverse(std::begin(sequence_3), std::end(sequence_3));
    std::cout << "Reversed sequence 3: " << sequence_3 << '\n';
    const ushort size_3 = sequence_3.size();
    std::nth_element(std::begin(sequence_3), std::prev(std::end(sequence_3)),
                     std::end(sequence_3));
    const int max_1_index = size_3 - 1;
    const int max_1 = sequence_3[max_1_index];
    std::nth_element(std::begin(sequence_3), std::prev(std::end(sequence_3), 2),
                     std::end(sequence_3));
    const int max_2_index = size_3 - 2;
    const int max_2 = sequence_3[max_2_index];
    std::nth_element(std::begin(sequence_3), std::prev(std::end(sequence_3), 3),
                     std::end(sequence_3));
    const int max_3_index = size_3 - 3;
    const int max_3 = sequence_3[max_3_index];
    std::cout << "3 max: " << max_3 << "; " << max_2 << "; " << max_1 << '\n';
    std::sort(std::begin(sequence_1), std::end(sequence_1));
    std::sort(std::begin(sequence_2), std::end(sequence_2));
    std::cout << "Sorted sequence 1: " << sequence_1 << '\n';
    std::cout << "Sorted sequence 2: " << sequence_2 << '\n';
    std::vector<int> sequence_4;
    std::merge(std::begin(sequence_1), std::end(sequence_1), std::begin(sequence_2),
               std::end(sequence_2), std::back_inserter(sequence_4));
    std::cout << "Sequence 4: " << sequence_4 << '\n';
    const ushort insert = 1;
    auto range = std::equal_range(std::begin(sequence_4), std::end(sequence_4), insert);
    auto first_it = range.first;
    auto second_it = range.second;
    ushort first = std::distance(std::begin(sequence_4), first_it);
    ushort second = std::distance(std::begin(sequence_4), second_it);
    std::cout << "Range to insert '1': " << first << " - " << second << '\n';
    std::cout << "Sequence 1: " << sequence_1 << '\n';
    std::cout << "Sequence 2: " << sequence_2 << '\n';
    std::cout << "Sequence 3: " << sequence_3 << '\n';
    std::cout << "Sequence 4: " << sequence_4 << '\n';
    return 0;
}
