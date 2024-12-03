#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <regex>
#include <cstdio>


#define MY_INPUT_PATH R"(..\inputDay3.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day3.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day3.txt)"


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::string corruptedMemory;
    for (std::string line; std::getline(input, line); corruptedMemory += line);

    long long sumOfMultiplications{};
    std::regex exp(R"(mul\(\d{1,3},\d{1,3}\))");
    std::smatch sm;

    for (
        std::string mutableLine{ corruptedMemory };
        std::regex_search(mutableLine, sm, exp);
        mutableLine = sm.suffix().str()
    ) {

        int leftNum, rightNum;
        sscanf(sm.str().c_str(), "mul(%d,%d)", &leftNum, &rightNum);

        sumOfMultiplications += leftNum * rightNum;
    }

    return sumOfMultiplications;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::string corruptedMemory;
    for (std::string line; std::getline(input, line); corruptedMemory += line);

    long long sumOfMultiplications{};
    std::regex exp(R"(mul\(\d{1,3},\d{1,3}\))");
    std::smatch sm;

    for (
        std::string mutableLine{ corruptedMemory }, beforeCurrentMul;
        std::regex_search(mutableLine, sm, exp);
        mutableLine = sm.suffix().str()
    ) {
        beforeCurrentMul += sm.prefix().str() + sm.str();

        std::size_t lastDoPosition{ beforeCurrentMul.rfind("do()") };
        std::size_t lastDontPosition{ beforeCurrentMul.rfind("don't()") };

        if (
            lastDontPosition == std::string::npos ||
            (lastDoPosition != std::string::npos &&
             lastDoPosition > lastDontPosition)
        ) {
            int leftNum, rightNum;
            sscanf(sm.str().c_str(), "mul(%d,%d)", &leftNum, &rightNum);

            sumOfMultiplications += leftNum * rightNum;
        }
    }

    return sumOfMultiplications;
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
