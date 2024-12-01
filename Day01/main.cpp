#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <set>
#include <algorithm>
#include <cmath>

#define MY_INPUT_PATH R"(..\inputDay1.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day1.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day1.txt)"


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::multiset<long long> leftIDs, rightIDs;
    for (
         long long left, right;
         input >> left >> right;
         leftIDs.insert(left), rightIDs.insert(right)
    );

    long long totalDistance{};
    for (
         auto lIter{ leftIDs.begin() }, rIter{ rightIDs.begin() };
         lIter != leftIDs.end() && rIter != rightIDs.end();
         totalDistance += std::abs(*lIter++ - *rIter++)
    );

    return totalDistance;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::multiset<long long> leftIDs, rightIDs;
    for (
         long long left, right;
         input >> left >> right;
         leftIDs.insert(left), rightIDs.insert(right)
    );

    long long totalSimilarityScore{};
    for (
         auto lIter{ leftIDs.begin() };
         lIter != leftIDs.end();
         totalSimilarityScore += *lIter * rightIDs.count(*lIter++)
    );

    return totalSimilarityScore;
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
