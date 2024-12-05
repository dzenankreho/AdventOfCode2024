#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>


#define MY_INPUT_PATH R"(..\inputDay5.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day5.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day5.txt)"


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::pair<int, int>> pageOrderingRules;
    for (std::string line; std::getline(input, line) && !line.empty(); ) {
        std::stringstream ss(line);

        int pageNumber1, pageNumber2;
        char vBar;

        ss >> pageNumber1 >> vBar >> pageNumber2;

        pageOrderingRules.emplace_back(pageNumber1, pageNumber2);
    }

    long long sumOfCorrectMiddlePageNumbers{};

    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss(line);
        char comma;
        std::vector<int> pageNumbers;

        for (
            int pageNumber;
            ss >> pageNumber && (ss.eof() || ss >> comma);
            pageNumbers.push_back(pageNumber)
        );

        bool correct{ true };

        for (const auto& [rulePageNumber1, rulePageNumber2] : pageOrderingRules) {
            if (auto pageNum1It{
                    std::find(pageNumbers.begin(), pageNumbers.end(), rulePageNumber1)
                }, pageNum2It {
                    std::find(pageNumbers.begin(), pageNumbers.end(), rulePageNumber2)
                };
                pageNum1It != pageNumbers.end() &&
                pageNum2It != pageNumbers.end() &&
                pageNum1It > pageNum2It
            ) {
                correct = false;
                break;
            }
        }

        if (correct) {
            sumOfCorrectMiddlePageNumbers += pageNumbers.at(pageNumbers.size() / 2);
        }
    }

    return sumOfCorrectMiddlePageNumbers;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::pair<int, int>> pageOrderingRules;
    for (std::string line; std::getline(input, line) && !line.empty(); ) {
        std::stringstream ss(line);

        int pageNumber1, pageNumber2;
        char vBar;

        ss >> pageNumber1 >> vBar >> pageNumber2;

        pageOrderingRules.emplace_back(pageNumber1, pageNumber2);
    }

    long long sumOfUpdatedMiddlePageNumbers{};

    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss(line);
        char comma;
        std::vector<int> pageNumbers;

        for (
            int pageNumber;
            ss >> pageNumber && (ss.eof() || ss >> comma);
            pageNumbers.push_back(pageNumber)
        );

        for (const auto &[rulePageNumber1, rulePageNumber2]: pageOrderingRules) {
            if (auto pageNum1It{
                    std::find(pageNumbers.begin(), pageNumbers.end(), rulePageNumber1)
                }, pageNum2It{
                    std::find(pageNumbers.begin(), pageNumbers.end(), rulePageNumber2)
                };
                pageNum1It != pageNumbers.end() &&
                pageNum2It != pageNumbers.end() &&
                pageNum1It > pageNum2It
            ) {
                std::sort(
                    pageNumbers.begin(),
                    pageNumbers.end(),
                    [&pageOrderingRules](int pageNum1, int pageNum2) {
                        return std::any_of(
                            pageOrderingRules.begin(),
                            pageOrderingRules.end(),
                            [pageNum1, pageNum2](auto &&rule) {
                                return pageNum1 == rule.first && pageNum2 == rule.second;
                            }
                        );
                    }
                );

                sumOfUpdatedMiddlePageNumbers += pageNumbers.at(pageNumbers.size() / 2);
                break;
            }
        }
    }

    return sumOfUpdatedMiddlePageNumbers;
}


double measureTime(const std::function<void()>& func, int numOfRuns) {
    auto startTime = std::chrono::steady_clock::now();
    for (int i{}; i < numOfRuns; i++) {
        func();
    }
    auto endTime = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / (double)numOfRuns;
}


int main() {
    std::cout << "Test inputs:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(TEST_INPUT_PART1_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(TEST_INPUT_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
