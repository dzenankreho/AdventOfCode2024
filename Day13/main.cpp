#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <cstdio>


#define MY_INPUT_PATH R"(..\inputDay13.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day13.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day13.txt)"


long long solution(const char* inputPath, long long prizeXYShift) {
    std::ifstream input(inputPath);

    long long numOfTokens{};

    for (std::string line; std::getline(input, line); ) {
        if (line.empty()) {
            continue;
        }

        long long aX, aY;
        std::sscanf(line.c_str(), "Button A: X+%lld, Y+%lld", &aX, &aY);

        std::getline(input, line);
        long long bX, bY;
        std::sscanf(line.c_str(), "Button B: X+%lld, Y+%lld", &bX, &bY);

        std::getline(input, line);
        long long prizeX, prizeY;
        std::sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &prizeX, &prizeY);

        prizeX += prizeXYShift;
        prizeY += prizeXYShift;

        if (long long  determinant{ aX * bY - aY * bX }; determinant != 0) {
            long long  numOfTokensA{ (prizeX * bY - prizeY * bX) / determinant };
            long long  numOfTokensB{ (aX * prizeY - aY * prizeX) / determinant };

            if (
                numOfTokensA >= 0 &&
                numOfTokensB >= 0 &&
                aX * numOfTokensA + bX * numOfTokensB == prizeX &&
                aY * numOfTokensA + bY * numOfTokensB == prizeY
            ) {
                numOfTokens += 3 * numOfTokensA + numOfTokensB;
            }
        }
    }

    return numOfTokens;
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
    std::cout << "\tPart 1: " << solution(TEST_INPUT_PART1_PATH, 0) << std::endl;
    std::cout << "\tPart 2: " << solution(TEST_INPUT_PART2_PATH, 10000000000000) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solution(MY_INPUT_PATH, 0) << std::endl;
    std::cout << "\tPart 2: " << solution(MY_INPUT_PATH, 10000000000000) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solution(MY_INPUT_PATH, 0); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solution(MY_INPUT_PATH, 10000000000000); }, 1000) << std::endl;

    return 0;
}
