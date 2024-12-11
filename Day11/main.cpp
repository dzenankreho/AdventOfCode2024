#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <unordered_map>


#define MY_INPUT_PATH R"(..\inputDay11.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day11.txt)"


long long solution(const char* inputPath, int numOfBlinks) {
    std::ifstream input(inputPath);

    std::unordered_map<long long, long long> stones;
    for (long long stone; input >> stone; ++stones[stone]);

    for (int blink{}; blink < numOfBlinks; ++blink) {
        std::unordered_map<long long, long long> stonesPreviousBlink{ stones };
        for (auto&& stone : stonesPreviousBlink) {
            if (stone.second == 0) {
                continue;
            }

            stones[stone.first] -= stone.second;

            if (stone.first == 0) {
                stones[1] += stone.second;
                continue;
            }

            int numOfDigits{};
            long long stoneCopy{ stone.first };
            do {
                stoneCopy /= 10;
                numOfDigits++;
            } while (stoneCopy != 0);

            if (numOfDigits % 2 == 0) {
                long long tens{ 1 };
                for (int i{}; i < numOfDigits / 2; ++i, tens *= 10);

                stones[stone.first / tens] += stone.second;
                stones[stone.first % tens] += stone.second;

                continue;
            }

            stones[stone.first * 2024] += stone.second;
        }
    }

    long long numOfStones{};
    for (auto&& stone : stones) {
        numOfStones += stone.second;
    }

    return numOfStones;
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
    std::cout << "\tPart 1: " << solution(TEST_INPUT_PART1_PATH, 25) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solution(MY_INPUT_PATH, 25) << std::endl;
    std::cout << "\tPart 2: " << solution(MY_INPUT_PATH, 75) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solution(MY_INPUT_PATH, 25); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solution(MY_INPUT_PATH, 75); }, 1000) << std::endl;

    return 0;
}
