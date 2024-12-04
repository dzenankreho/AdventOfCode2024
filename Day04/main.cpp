#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <vector>


#define MY_INPUT_PATH R"(..\inputDay4.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day4.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day4.txt)"


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> wordSearch;
    for (std::string line; std::getline(input, line); wordSearch.push_back(line));

    std::string xmas{ "XMAS" };
    long long xmasCount{};

    for (int i{}; i < wordSearch.size(); ++i) {
        for (int j{}; j < wordSearch.at(i).size(); ++j) {
            if (j + 3 < wordSearch.at(i).size()) {
                bool matches{ true }, matchesReverse{ true };
                for (int k{}; k < 4; ++k) {
                    matches &= wordSearch.at(i).at(j + k) == xmas.at(k);
                    matchesReverse &= wordSearch.at(i).at(j + k) == xmas.at(xmas.length() - 1 - k);
                }

                xmasCount += matches | matchesReverse;
            }

            if (i + 3 < wordSearch.size()) {
                bool matches{ true }, matchesReverse{ true };
                for (int k{}; k < 4; ++k) {
                    matches &= wordSearch.at(i + k).at(j) == xmas.at(k);
                    matchesReverse &= wordSearch.at(i + k).at(j) == xmas.at(xmas.length() - 1 - k);
                }

                xmasCount += matches | matchesReverse;
            }

            if (i + 3 < wordSearch.size() && j + 3 < wordSearch.at(i).size()) {
                bool matches{ true }, matchesReverse{ true };
                for (int k{}; k < 4; ++k) {
                    matches &= wordSearch.at(i + k).at(j + k) == xmas.at(k);
                    matchesReverse &= wordSearch.at(i + k).at(j + k) == xmas.at(xmas.length() - 1 - k);
                }

                xmasCount += matches | matchesReverse;
            }

            if (i >= 3 && j + 3 < wordSearch.at(i).size()) {
                bool matches{ true }, matchesReverse{ true };
                for (int k{}; k < 4; ++k) {
                    matches &= wordSearch.at(i - k).at(j + k) == xmas.at(k);
                    matchesReverse &= wordSearch.at(i - k).at(j + k) == xmas.at(xmas.length() - 1 - k);
                }

                xmasCount += matches | matchesReverse;
            }
        }
    }

    return xmasCount;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> wordSearch;
    for (std::string line; std::getline(input, line); wordSearch.push_back(line));

    std::string mas{ "MAS" };
    long long xmasCount{};

    for (int i{ 1 }; i + 1 < wordSearch.size(); ++i) {
        for (int j{ 1 }; j + 1 < wordSearch.at(i).size(); ++j) {
            bool matchesFirstDiag{ true }, matchesReverseFirstDiag{ true };
            for (int k{}; k < 3; ++k) {
                matchesFirstDiag &= wordSearch.at(i - 1 + k).at(j - 1 + k) == mas.at(k);
                matchesReverseFirstDiag &= wordSearch.at(i - 1 + k).at(j - 1 + k) == mas.at(mas.length() - 1 - k);
            }

            bool matchesSecondDiag{ true }, matchesReverseSecondDiag{ true };
            for (int k{}; k < 3; ++k) {
                matchesSecondDiag &= wordSearch.at(i + 1 - k).at(j - 1 + k) == mas.at(k);
                matchesReverseSecondDiag &= wordSearch.at(i + 1 - k).at(j - 1 + k) == mas.at(mas.length() - 1 - k);
            }

            xmasCount += (matchesFirstDiag | matchesReverseFirstDiag) &
                         (matchesSecondDiag | matchesReverseSecondDiag);
        }
    }

    return xmasCount;
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
