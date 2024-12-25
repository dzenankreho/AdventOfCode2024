#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <vector>


#define MY_INPUT_PATH R"(..\inputDay25.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day25.txt)"


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::vector<int>> lockPinHeights, keyHeights;
    int maxLockPinAndKeyHeight{};

    for (; !input.eof(); ) {
        std::vector<std::string> schematic;
        for (std::string line; std::getline(input, line) && !line.empty(); schematic.push_back(line));

        if (maxLockPinAndKeyHeight == 0) {
            maxLockPinAndKeyHeight = schematic.size() - 1;
        }

        if (schematic.front() == std::string(schematic.front().size(), '#')) {
            lockPinHeights.emplace_back(schematic.front().size(), 0);
            for (int i{ 1 }; i + 1 < schematic.size(); ++i) {
                for (int j{}; j < schematic.at(i).size(); ++j) {
                    lockPinHeights.back().at(j) += schematic.at(i).at(j) == '#';
                }
            }
        }

        if (schematic.front() == std::string(schematic.front().size(), '.')) {
            keyHeights.emplace_back(schematic.front().size(), 0);
            for (int i{ 1 }; i + 1 < schematic.size(); ++i) {
                for (int j{}; j < schematic.at(i).size(); ++j) {
                    keyHeights.back().at(j) += schematic.at(i).at(j) == '#';
                }
            }
        }
    }

    int numOfFittingLockKeyPairs{};
    for (const std::vector<int>& lockPin : lockPinHeights) {
        for (const std::vector<int>& key : keyHeights) {
            bool fitting{ true };

            for (int i{}; i < key.size() && fitting; ++i) {
                fitting &= key.at(i) + lockPin.at(i) < maxLockPinAndKeyHeight;
            }

            numOfFittingLockKeyPairs += fitting;
        }
    }

    return numOfFittingLockKeyPairs;
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
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
