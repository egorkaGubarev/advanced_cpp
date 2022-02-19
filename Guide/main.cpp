#include <iostream>
#include <map>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

typedef unsigned int uint;

class UniformRandomGenerator
{
public:
    UniformRandomGenerator(): UniformRandomGenerator(-1) {}

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

struct Name
{
    Name() = default;

    Name(const std::string& surname, const std::string& name, const std::string& patronymic)
    {
        std::get<0>(name_) = surname;
        std::get<1>(name_) = name;
        std::get<2>(name_) = patronymic;
    }

    friend bool operator==(const Name& lhs, const Name& rhs)
    {
        return lhs.name_ == rhs.name_;
    }

    friend bool operator<(const Name& lhs, const Name& rhs)
    {
        return lhs.name_ < rhs.name_;
    }

    friend std::ostream& operator << (std::ostream& out, const Name& arg)
    {
        const std::string& surname = std::get<0>(arg.name_);
        const std::string& name = std::get<1>(arg.name_);
        const std::string& patronymic = std::get<2>(arg.name_);
        std::cout << surname << ' ' << name << ' ' << patronymic;
        return out;
    }

    std::tuple<std::string, std::string, std::string> name_;
};

class Name_Hash{
public:
    uint operator()(const Name& name) const
    {
        const uint name_hash = std::hash<std::string>{}(std::get<1>(name.name_));
        const uint surname_hash = std::hash<std::string>{}(std::get<0>(name.name_));
        const uint patronymic_hash = std::hash<std::string>{}(std::get<2>(name.name_));
        const uint hash = name_hash + surname_hash + patronymic_hash;
        return hash;
    }
};

class Guide{
public:
    Guide() = default;

    void add(const std::string& name, const std::string& surname,
             const std::string& patronymic, const std::string& phone)
    {
        const Name name_struct{name, surname, patronymic};
        const std::pair<Name, std::string> record{name_struct, phone};
        data_.insert(record);
        sorted_data_.insert(record);
    }

    const std::map<const Name, const std::string>& get_sorted() const
    {
        return sorted_data_;
    }

    std::pair<const Name&, const std::string&> get_random()
    {
        const uint size = sorted_data_.size();
        const uint delta_max = size - 1;
        const std::vector<int> random = generator_.create_sequence(1, 0, (int)delta_max);
        const uint delta = random[0];
        auto random_iterator = std::begin(sorted_data_);
        std::advance(random_iterator, delta);
        const std::pair<const Name&, const std::string&> random_record = *random_iterator;
        return random_record;
    }

    std::string get_phone(const std::string& surname, const std::string& name, const std::string& patronymic)
    {
        const Name target(surname, name, patronymic);
        auto find_it = data_.find(target);
        const std::pair<Name, std::string> record = *find_it;
        std::string phone = record.second;
        return phone;
    }

private:
    std::unordered_map<Name, std::string, Name_Hash> data_;
    std::map<const Name, const std::string> sorted_data_;
    UniformRandomGenerator generator_;
};

int main()
{
    Guide guide;
    guide.add("Gubarev", "Egorka", "Dmitrievich", "684217782");
    guide.add("Vorobyova", "Sofia", "Egorovna", "632525656");
    guide.add("Goncharova", "Darya", "Glebovna", "132706599");
    guide.add("Chernova", "Vasilisa", "Nikitichna", "782072079");
    guide.add("Rodin", "Alexey", "Alexandrovich", "869584655");
    const std::map<const Name, const std::string>& sorted = guide.get_sorted();
    std::cout << "Sorted:" << '\n';
    for(const std::pair<const Name, const std::string>& record: sorted){
        const Name& name = record.first;
        const std::string& phone = record.second;
        std::cout << "Name: " << name << "; Phone: " << phone << '\n';
    }
    const std::pair<const Name, const std::string> record = guide.get_random();
    const Name& name = record.first;
    const std::string& phone = record.second;
    std::cout << "Random:" << '\n';
    std::cout << "Name: " << name << "; Phone: " << phone << '\n';
    const std::string& my_phone = guide.get_phone("Gubarev", "Egorka", "Dmitrievich");
    std::cout << "Search:" << '\n';
    std::cout << "My phone: " << my_phone;
    return 0;
}
