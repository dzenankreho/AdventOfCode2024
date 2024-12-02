#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>


#define MY_INPUT_PATH R"(..\inputDay2.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day2.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day2.txt)"


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    int numOfSafeReports{};
    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss(line);

        std::vector<int> levels;
        for (int level{}; ss >> level; levels.push_back(level));

        std::vector<int> levelsDiff(levels.size());
        std::adjacent_difference(levels.begin(), levels.end(), levelsDiff.begin());
        levelsDiff.erase(levelsDiff.begin());
        bool negativeDiff{ levelsDiff.front() < 0 };

        numOfSafeReports += std::all_of(
            levelsDiff.begin(),
            levelsDiff.end(),
            [negativeDiff] (int levelDiff) {
                return (!negativeDiff && levelDiff >= 1 && levelDiff <= 3)
                    || (negativeDiff && levelDiff <= -1 && levelDiff >= -3);
            }
        );

    }

    return numOfSafeReports;
}


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    int numOfSafeReports{};
    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss(line);

        std::vector<int> levels;
        for (int level{}; ss >> level; levels.push_back(level));

        for (int removeIdx{ -1 }; removeIdx < int(levels.size()); ++removeIdx) {
            std::vector<int> levelsModified(levels);
            if (removeIdx != -1) {
                levelsModified.erase(levelsModified.begin() + removeIdx);
            }

            std::vector<int> levelsDiff(levelsModified.size());
            std::adjacent_difference(levelsModified.begin(), levelsModified.end(), levelsDiff.begin());
            levelsDiff.erase(levelsDiff.begin());
            bool negativeDiff{ levelsDiff.front() < 0 };

            if (std::all_of(
                    levelsDiff.begin(),
                    levelsDiff.end(),
                    [negativeDiff] (int levelDiff) {
                        return (!negativeDiff && levelDiff >= 1 && levelDiff <= 3)
                               || (negativeDiff && levelDiff <= -1 && levelDiff >= -3);
                    }
                )
            ) {

                ++numOfSafeReports;
                break;
            }
        }
    }

    return numOfSafeReports;
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
