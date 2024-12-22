#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include <cmath>


#define MY_INPUT_PATH R"(..\inputDay21.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day21.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day21.txt)"


struct Position {
    int x;
    int y;

    Position operator-(const Position& position) const {
        return { x - position.x, y - position.y };
    }
};


template <>
struct std::hash<std::pair<std::string, int>> {
    std::size_t operator()(const std::pair<std::string, int>& pair) const {
        return std::hash<std::string>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
    }
};


using KeypadTransitionTable = std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>>;


long long countNumOfHumanButtonPresses(
    const KeypadTransitionTable& directionalKeypadTransitions,
    const std::string& currentDirKeypadInput,
    const int numOfRobotsUsingDirKeypads,
    std::unordered_map<std::pair<std::string, int>, long long>& inputCache
) {
    std::pair<std::string, int> input{ currentDirKeypadInput, numOfRobotsUsingDirKeypads };

    if (inputCache.contains(input)) {
        return inputCache.at(input);
    }

    long long numOfButtonPresses{};
    char currentDirKeypadPosition{ 'A' };

    if (numOfRobotsUsingDirKeypads == 1) {
        for (char dirKeypadButton : currentDirKeypadInput) {
            numOfButtonPresses +=
                directionalKeypadTransitions.at(currentDirKeypadPosition).at(dirKeypadButton).front().length();

            currentDirKeypadPosition = dirKeypadButton;
        }
    } else {
        for (char dirKeypadButton : currentDirKeypadInput) {
            numOfButtonPresses += countNumOfHumanButtonPresses(
                directionalKeypadTransitions,
                directionalKeypadTransitions.at(currentDirKeypadPosition).at(dirKeypadButton).front(),
                numOfRobotsUsingDirKeypads - 1,
                inputCache
            );

            currentDirKeypadPosition = dirKeypadButton;
        }
    }

    inputCache.emplace(input, numOfButtonPresses);
    return numOfButtonPresses;
}


long long solution(const char* inputPath, const int numOfRobotsUsingDirKeypads) {
    std::ifstream input(inputPath);

    std::unordered_map<char, Position> numericKeypad{
            { '0', { 3, 1 } }, { 'A', { 3, 2 } },
            { '1', { 2, 0 } }, { '2', { 2, 1 } }, { '3', { 2, 2 } },
            { '4', { 1, 0 } }, { '5', { 1, 1 } }, { '6', { 1, 2 } },
            { '7', { 0, 0 } }, { '8', { 0, 1 } }, { '9', { 0, 2 } }
    };

    KeypadTransitionTable numericKeypadTransitions;
    for (const auto& [fromValue, fromPosition] : numericKeypad) {
        std::unordered_map<char, std::vector<std::string>> transitions;
        for (const auto& [toValue, toPosition] : numericKeypad) {
            Position diff{ fromPosition - toPosition };

            std::string possiblePreviousDirKeypadInput1{
                std::string(std::abs(diff.x), ((diff.x >= 0) ? ('^') : ('v'))) +
                std::string(std::abs(diff.y), ((diff.y >= 0) ? ('<') : ('>')))
            };
            std::string possiblePreviousDirKeypadInput2{
                possiblePreviousDirKeypadInput1.rbegin(), possiblePreviousDirKeypadInput1.rend()
            };

            possiblePreviousDirKeypadInput1 += 'A';
            possiblePreviousDirKeypadInput2 += 'A';

            std::vector<std::string> possiblePreviousDirKeypadInputs;
            if (diff.x == 0 || diff.y == 0 || (diff.x > 0 && fromPosition.x == 3 && toPosition.y == 0)) {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput1);
            } else if (diff.x < 0 && fromPosition.y == 0 && toPosition.x == 3) {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput2);
            } else {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput1);
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput2);
            }

            transitions.emplace(toValue, possiblePreviousDirKeypadInputs);
        }

        numericKeypadTransitions.emplace(fromValue, transitions);
    }

    std::unordered_map<char, Position> directionalKeypad{
            { '<', { 1, 0 } }, { 'v', { 1, 1 } }, { '>', { 1, 2 } },
            { '^', { 0, 1 } }, { 'A', { 0, 2 } }
    };

    KeypadTransitionTable directionalKeypadTransitions;
    for (const auto& [fromValue, fromPosition] : directionalKeypad) {
        std::unordered_map<char, std::vector<std::string>> transitions;
        for (const auto& [toValue, toPosition] : directionalKeypad) {
            Position diff{ fromPosition - toPosition };

            std::string possiblePreviousDirKeypadInput1{
                std::string(std::abs(diff.x), ((diff.x >= 0) ? ('^') : ('v'))) +
                std::string(std::abs(diff.y), ((diff.y >= 0) ? ('<') : ('>')))
            };
            std::string possiblePreviousDirKeypadInput2{
                possiblePreviousDirKeypadInput1.rbegin(),
                possiblePreviousDirKeypadInput1.rend()
            };

            possiblePreviousDirKeypadInput1 += 'A';
            possiblePreviousDirKeypadInput2 += 'A';

            std::vector<std::string> possiblePreviousDirKeypadInputs;
            if (diff.x == 0 || diff.y == 0 || (diff.x > 0 && fromPosition.y == 0 && toPosition.x == 0)) {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput2);
            } else if (diff.x < 0 && fromPosition.x == 0 && toPosition.y == 0) {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput1);
            } else {
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput1);
                possiblePreviousDirKeypadInputs.push_back(possiblePreviousDirKeypadInput2);
            }

            transitions.emplace(toValue, possiblePreviousDirKeypadInputs);
        }

        directionalKeypadTransitions.emplace(fromValue, transitions);
    }


    std::unordered_map<std::pair<std::string, int>, long long> inputCache;

    for (auto& [fromValue, transitions] : numericKeypadTransitions) {
        for (auto& [toValue, possiblePreviousDirKeypadInputs] : transitions) {
            if (possiblePreviousDirKeypadInputs.size() == 1) {
                continue;
            }

            if (
                countNumOfHumanButtonPresses(
                    directionalKeypadTransitions,
                    possiblePreviousDirKeypadInputs.front(),
                    2,
                    inputCache
                )
                >
                countNumOfHumanButtonPresses(
                    directionalKeypadTransitions,
                    possiblePreviousDirKeypadInputs.back(),
                    2,
                    inputCache
                )
            ) {
                possiblePreviousDirKeypadInputs.erase(possiblePreviousDirKeypadInputs.begin());
            } else {
                possiblePreviousDirKeypadInputs.pop_back();
            }
        }
    }

    for (auto& [fromValue, transitions] : directionalKeypadTransitions) {
        for (auto& [toValue, possiblePreviousDirKeypadInputs] : transitions) {
            if (possiblePreviousDirKeypadInputs.size() == 1) {
                continue;
            }

            if (
                countNumOfHumanButtonPresses(
                    directionalKeypadTransitions,
                    possiblePreviousDirKeypadInputs.front(),
                    2,
                    inputCache
                )
                >
                countNumOfHumanButtonPresses(
                    directionalKeypadTransitions,
                    possiblePreviousDirKeypadInputs.back(),
                    2,
                    inputCache
                )
            ) {
                possiblePreviousDirKeypadInputs.erase(possiblePreviousDirKeypadInputs.begin());
            } else {
                possiblePreviousDirKeypadInputs.pop_back();
            }
        }
    }


    long long sumOfComplexities{};
    for (std::string line; std::getline(input, line); ) {
        long long sequenceLength{};
        char currentKeypadPosition{ 'A' };

        for (char numKeypadButton : line) {
            sequenceLength += countNumOfHumanButtonPresses(
                    directionalKeypadTransitions,
                    numericKeypadTransitions.at(currentKeypadPosition).at(numKeypadButton).front(),
                    numOfRobotsUsingDirKeypads,
                    inputCache
            );

            currentKeypadPosition = numKeypadButton;
        }

        line.pop_back();
        sumOfComplexities += sequenceLength * std::stoi(line);
    }

    return sumOfComplexities;
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
    std::cout << "\tPart 1: " << solution(TEST_INPUT_PART1_PATH, 2) << std::endl;
    std::cout << "\tPart 2: " << solution(TEST_INPUT_PART2_PATH, 25) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solution(MY_INPUT_PATH, 2) << std::endl;
    std::cout << "\tPart 2: " << solution(MY_INPUT_PATH, 25) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solution(MY_INPUT_PATH, 2); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solution(MY_INPUT_PATH, 25); }, 1000) << std::endl;

    return 0;
}
