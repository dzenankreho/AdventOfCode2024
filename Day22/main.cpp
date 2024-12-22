#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <deque>
#include <vector>


#define MY_INPUT_PATH R"(..\inputDay22.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day22.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day22.txt)"


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    long long sumOfNewSecretNums{};

    for (long long secretNum; input >> secretNum; ) {
        for (int i{}; i < 2000; ++i) {
            secretNum ^= secretNum * 64;
            secretNum %= 16777216;
            secretNum ^= secretNum / 32;
            secretNum %= 16777216;
            secretNum ^= secretNum * 2048;
            secretNum %= 16777216;
        }
        sumOfNewSecretNums += secretNum;
    }

    return sumOfNewSecretNums;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<long long> initSecretNums;
    for (long long secretNum; input >> secretNum; initSecretNums.push_back(secretNum));

    std::vector<std::vector<int>> allPossibleSequenceOfFourPriceChanges;
    for (int i{ -9 }; i <= 9; ++i) {
        for (int j{ -9 }; j <= 9; ++j) {
            for (int k{ -9 }; k <= 9; ++k) {
                for (int l{ -9 }; l <= 9; ++l) {
                    allPossibleSequenceOfFourPriceChanges.push_back({ i, j, k, l });
                }
            }
        }
    }

    long long maxNumOfBananas{};
    for (const std::vector<int>& sequenceOfFourPriceChanges : allPossibleSequenceOfFourPriceChanges) {
        long long numOfBananas{};
        for (long long secretNum : initSecretNums) {
            std::deque<int> lastFivePrices;
            for (int i{}; i < 2000; ++i) {
                lastFivePrices.push_back(secretNum % 10);
                if (lastFivePrices.size() > 5) {
                    lastFivePrices.pop_front();

                    bool sequenceFound{ true };
                    for (int j{ 1 }; j < 5; ++j) {
                        if (
                            lastFivePrices.at(j) - lastFivePrices.at(j - 1) !=
                                sequenceOfFourPriceChanges.at(j - 1)
                        ) {
                            sequenceFound = false;
                            break;
                        }
                    }

                    if (sequenceFound) {
                        numOfBananas += lastFivePrices.back();
                    }
                }

                secretNum ^= secretNum * 64;
                secretNum %= 16777216;
                secretNum ^= secretNum / 32;
                secretNum %= 16777216;
                secretNum ^= secretNum * 2048;
                secretNum %= 16777216;
            }
        }

        if (numOfBananas > maxNumOfBananas) {
            std::cout << numOfBananas << " - ";
            for (auto j : sequenceOfFourPriceChanges) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
            maxNumOfBananas = numOfBananas;
        }
    }

    return maxNumOfBananas;
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
//    std::cout << "\tPart 2: " << solutionPart2(TEST_INPUT_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
//    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
//    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
