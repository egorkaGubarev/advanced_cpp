#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> search(const std::regex& pattern, const std::string& text)
{
    auto begin = std::sregex_iterator(std::begin(text), std::end(text), pattern);
    auto end = std::sregex_iterator();
    std::vector<std::string> results(std::distance(begin, end));
    std::transform(begin, end, std::back_inserter(results), [](auto x){return x.str();});
    return results;
}

int main()
{
    const std::regex date(R"([0-9]{4}\.((([0]{1}[13578]{1}\.(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})|([3]{1}[0-1]{1})))|([0]{1}[2]{1}\.(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})))|([0]{1}[469]{1}\.(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})|([3]{1}[0]{1}))))|(([1]{1}[02]{1}\.(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})|([3]{1}[0-1]{1})))|([1]{1}[1]{1}\.(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})|([3]{1}[0]{1}))))))");
    // It looks very scary
    // Am I doing all right? )
    // It still doesn't check if the year is leap or not. If february has less than 30 days, it's counts as correct date
    // I'm afraid of how large is the expression which checks this fact
    const std::string dates = "2022.03.05 \n"
                             "2022.01.10 \n"
                             "2022.01.30 \n"
                             "2022.02.01 \n"
                             "2022.02.28 \n"
                             "2022.02.29 \n"
                             "2022.03.31 \n"
                             "2022.04.01 \n"
                             "2022.05.31 \n"
                             "2022.06.31 \n"
                             "2022.07.31 \n"
                             "2022.08.31 \n"
                             "2022.09.30 \n"
                             "2022.10.01 \n"
                             "2022.11.30 \n"
                             "2022.12.31 \n"
                             "2022.04.31 \n"
                             "2022.01.00 \n"
                             "2022.00.01 \n";
    std::vector<std::string> dates_correct = search(date, dates);
    std::cout << "Correct dates:" << '\n';
    std::copy(std::begin(dates_correct), std::end(dates_correct),
              std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << '\n';
    const std::regex time(R"((([0-1]{1}[0-9]{1})|([2]{1}[0-3]{1}))(:[0-5]{1}[0-9]{1}){2})");
    const std::string times = "00:00:00 \n"
                              "00:00:59 \n"
                              "23:00:00 \n"
                              "26:01:55 \n"
                              "09:05:01 \n"
                              "01:36:40 \n"
                              "24:32:35 \n"
                              "16:30:03 \n"
                              "05:41:48 \n";
    std::vector<std::string> times_correct = search(time, times);
    std::cout << "Correct times:" << '\n';
    std::copy(std::begin(times_correct), std::end(times_correct),
              std::ostream_iterator<std::string>(std::cout, " "));
    return 0;
}
