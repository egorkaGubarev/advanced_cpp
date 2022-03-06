#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "json.hpp"

typedef unsigned short ushort;

class Data{
public:
    Data(): age_{}, hometown_{}, name_{}, surname_{}, university_{} {};

    [[nodiscard]] nlohmann::json to_json() const
    {
        nlohmann::json person;
        person["age"] = age_;
        person["hometown"] = hometown_;
        person["name"] = name_;
        person["surname"] = surname_;
        person["university"] = university_;
        return person;
    }

    friend std::istream& operator>>(std::istream& in, Data& data)
    {
        std::cout << "Enter name: " << '\n';
        in >> data.name_;
        std::cout << "Enter surname: " << '\n';
        in >> data.surname_;
        std::cout << "Enter age: " << '\n';
        in >> data.age_;
        std::cout << "Enter hometown: " << '\n';
        in >> data.hometown_;
        std::cout << "Enter university: " << '\n';
        in >> data.university_;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Data& data)
    {
        out << "Name: " << data.name_ << '\n';
        out << "Surname: " << data.surname_ << '\n';
        out << "Age: " << data.age_ << '\n';
        out << "Hometown: " << data.hometown_ << '\n';
        out << "University: " << data.university_ << '\n';
        return out;
    }

private:
    ushort age_;
    std::string hometown_, name_, surname_, university_;
};

int main() {
    Data data_1, data_2;
    std::cin >> data_1;
    std::cin >> data_2;
    const std::filesystem::path persons{"../persons"};
    std::filesystem::create_directory(persons);
    const nlohmann::json object_1 = data_1.to_json();
    const nlohmann::json object_2 = data_2.to_json();
    const std::filesystem::path dump_1 = persons / "person_1.json";
    const std::filesystem::path dump_2 = persons / "person_2.json";
    std::ofstream out_1, out_2;
    out_1.open(dump_1);
    out_2.open(dump_2);
    out_1 << object_1;
    out_2 << object_2;
    out_1.close();
    out_2.close();
    return 0;
}
