#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <unordered_set>


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


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<int, int> sequencesOfFourPriceChangesCount;
    int bestSequenceOfFourPriceChanges{};

    for (long long secretNum; input >> secretNum; ) {
        int sequenceOfFourPriceChanges{};
        int previousPrice{ int(secretNum % 10) };
        std::unordered_set<int> sequencesOfFourPriceChanges;

        for (int i{}; i < 2000; ++i) {
            secretNum ^= secretNum * 64;
            secretNum %= 16777216;
            secretNum ^= secretNum / 32;
            secretNum %= 16777216;
            secretNum ^= secretNum * 2048;
            secretNum %= 16777216;

            sequenceOfFourPriceChanges *= 100;
            sequenceOfFourPriceChanges += (secretNum % 10 - previousPrice % 10) + 10;

            previousPrice = secretNum % 10;

            if (sequenceOfFourPriceChanges / 100000000) {
                sequenceOfFourPriceChanges %= 100000000;

                if (!sequencesOfFourPriceChanges.contains(sequenceOfFourPriceChanges)) {
                    sequencesOfFourPriceChangesCount[sequenceOfFourPriceChanges] += previousPrice;
                    sequencesOfFourPriceChanges.insert(sequenceOfFourPriceChanges);

                    if (
                        sequencesOfFourPriceChangesCount[sequenceOfFourPriceChanges] >
                            sequencesOfFourPriceChangesCount[bestSequenceOfFourPriceChanges]
                    ) {
                        bestSequenceOfFourPriceChanges = sequenceOfFourPriceChanges;
                    }
                }
            }
        }
    }

    return sequencesOfFourPriceChangesCount[bestSequenceOfFourPriceChanges];
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
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 100) << std::endl;

    return 0;
}
