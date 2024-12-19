#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>


#define MY_INPUT_PATH R"(..\inputDay19.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day19.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day19.txt)"


bool isDesignPossible(
    const std::vector<std::string>& towelPatterns,
    std::unordered_map<std::string, bool>& designsCache,
    const std::string& design
) {
    if (design.empty()) {
        return true;
    }

    if (designsCache.contains(design)) {
        return designsCache.at(design);
    }

    for (const std::string& towelPattern : towelPatterns) {
        if (
            design.compare(0, towelPattern.length(), towelPattern) == 0 &&
            isDesignPossible(towelPatterns, designsCache, design.substr(towelPattern.length()))
        ) {
            designsCache.emplace(design, true);
            return true;
        }
    }

    designsCache.emplace(design, false);
    return false;
}


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> towelPatterns;
    for (std::string towelPattern; input >> towelPattern; ) {
        towelPatterns.push_back(towelPattern);

        if (towelPattern.back() != ',') {
            break;
        }

        towelPatterns.back().pop_back();
    }

    int numOfPossibleDesigns{};
    std::unordered_map<std::string, bool> designsCache;

    for (std::string design; std::getline(input, design); ) {
        if (design.empty()) {
            continue;
        }

        numOfPossibleDesigns += isDesignPossible(towelPatterns, designsCache, design);
    }

    return numOfPossibleDesigns;
}


long long numOfPossibleTowelArrangements(
    const std::vector<std::string>& towelPatterns,
    std::unordered_map<std::string, long long>& numOfArrangementsCache,
    const std::string& design
) {
    if (design.empty()) {
        return 1;
    }

    if (numOfArrangementsCache.contains(design)) {
        return numOfArrangementsCache.at(design);
    }

    long long numOfArrangements{};
    for (const std::string& towelPattern : towelPatterns) {
        if (design.compare(0, towelPattern.length(), towelPattern) == 0) {
            numOfArrangements += numOfPossibleTowelArrangements(
                towelPatterns,
                numOfArrangementsCache,
                design.substr(towelPattern.size())
            );
        }
    }

    numOfArrangementsCache.emplace(design, numOfArrangements);
    return numOfArrangements;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> towelPatterns;
    for (std::string towelPattern; input >> towelPattern; ) {
        towelPatterns.push_back(towelPattern);

        if (towelPattern.back() != ',') {
            break;
        }

        towelPatterns.back().pop_back();
    }

    long long numOfPossibleDesigns{};
    std::unordered_map<std::string, long long> numOfArrangementsCache;

    for (std::string design; std::getline(input, design); ) {
        if (design.empty()) {
            continue;
        }

        numOfPossibleDesigns += numOfPossibleTowelArrangements(towelPatterns, numOfArrangementsCache, design);
    }

    return numOfPossibleDesigns;
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
