#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <unordered_map>
#include <limits>


#define MY_INPUT_PATH R"(..\inputDay21.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day21.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day21.txt)"


struct Position {
    int x;
    int y;

    Position operator+(const Position& position) const {
        return { x + position.x, y + position.y };
    }

    Position operator-(const Position& position) const {
        return { x - position.x, y - position.y };
    }

    bool operator==(const Position& position) const {
        return x == position.x && y == position.y;
    }
};


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<char, Position> numericKeypad{
                           { '0', { 3, 1 } }, { 'A', { 3, 2 } },
        { '1', { 2, 0 } }, { '2', { 2, 1 } }, { '3', { 2, 2 } },
        { '4', { 1, 0 } }, { '5', { 1, 1 } }, { '6', { 1, 2 } },
        { '7', { 0, 0 } }, { '8', { 0, 1 } }, { '9', { 0, 2 } }
    };

    std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>> numericKeypadTransitions;
    for (const auto& [fromValue, fromPosition] : numericKeypad) {
        std::unordered_map<char, std::vector<std::string>> transitions;
        for (const auto& [toValue, toPosition] : numericKeypad) {
            std::vector<std::string> possiblePreviousRobotInputs;

            Position diff{ fromPosition - toPosition };

            std::string possiblePrevInput1;
            if (diff.x >= 0) {
                possiblePrevInput1 += std::string(diff.x, '^');
            } else {
                possiblePrevInput1 += std::string(-diff.x, 'v');
            }
            if (diff.y >= 0) {
                possiblePrevInput1 += std::string(diff.y, '<');
            } else {
                possiblePrevInput1 += std::string(-diff.y, '>');
            }
            possiblePrevInput1 += 'A';

            std::string possiblePrevInput2;
            if (diff.y >= 0) {
                possiblePrevInput2 += std::string(diff.y, '<');
            } else {
                possiblePrevInput2 += std::string(-diff.y, '>');
            }
            if (diff.x >= 0) {
                possiblePrevInput2 += std::string(diff.x, '^');
            } else {
                possiblePrevInput2 += std::string(-diff.x, 'v');
            }
            possiblePrevInput2 += 'A';

            if (diff.x == 0 || diff.y == 0 || (diff.x > 0 && fromPosition.x == 3 && toPosition.y == 0)) {
                possiblePreviousRobotInputs.push_back(possiblePrevInput1);
            } else if (diff.x < 0 && fromPosition.y == 0 && toPosition.x == 3) {
                possiblePreviousRobotInputs.push_back(possiblePrevInput2);
            } else {
                possiblePreviousRobotInputs.push_back(possiblePrevInput1);
                possiblePreviousRobotInputs.push_back(possiblePrevInput2);
            }

            transitions.emplace(toValue, possiblePreviousRobotInputs);
        }

        numericKeypadTransitions.emplace(fromValue, transitions);
    }

    std::unordered_map<char, Position> directionalKeypad{
            { '<', { 1, 0 } }, { 'v', { 1, 1 } }, { '>', { 1, 2 } },
            { '^', { 0, 1 } }, { 'A', { 0, 2 } }
    };

    std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>> directionalKeypadTransitions;
    for (const auto& [fromValue, fromPosition] : directionalKeypad) {
        std::unordered_map<char, std::vector<std::string>> transitions;
        for (const auto& [toValue, toPosition] : directionalKeypad) {
            std::vector<std::string> possiblePreviousRobotInputs;

            Position diff{ fromPosition - toPosition };

            std::string possiblePrevInput1;
            if (diff.x >= 0) {
                possiblePrevInput1 += std::string(diff.x, '^');
            } else {
                possiblePrevInput1 += std::string(-diff.x, 'v');
            }
            if (diff.y >= 0) {
                possiblePrevInput1 += std::string(diff.y, '<');
            } else {
                possiblePrevInput1 += std::string(-diff.y, '>');
            }
            possiblePrevInput1 += 'A';

            std::string possiblePrevInput2;
            if (diff.y >= 0) {
                possiblePrevInput2 += std::string(diff.y, '<');
            } else {
                possiblePrevInput2 += std::string(-diff.y, '>');
            }
            if (diff.x >= 0) {
                possiblePrevInput2 += std::string(diff.x, '^');
            } else {
                possiblePrevInput2 += std::string(-diff.x, 'v');
            }
            possiblePrevInput2 += 'A';

            if (diff.x == 0 || diff.y == 0 || (diff.x > 0 && fromPosition.y == 0 && toPosition.x == 0)) {
                possiblePreviousRobotInputs.push_back(possiblePrevInput2);
            } else if (diff.x < 0 && fromPosition.x == 0 && toPosition.y == 0) {
                possiblePreviousRobotInputs.push_back(possiblePrevInput1);
            } else {
                possiblePreviousRobotInputs.push_back(possiblePrevInput1);
                possiblePreviousRobotInputs.push_back(possiblePrevInput2);
            }

            transitions.emplace(toValue, possiblePreviousRobotInputs);
        }

        directionalKeypadTransitions.emplace(fromValue, transitions);
    }

    int sumOfComplexities{};

    for (std::string line; std::getline(input, line); ) {
        char currentKeypadPosition{ 'A' };

        int sequenceLength{};

        for (char numPadButton : line) {
            int minSequenceLength{ std::numeric_limits<int>::max() };

            for (
                const std::string& possibleTransitionsRobot1 :
                    numericKeypadTransitions.at(currentKeypadPosition).at(numPadButton)
            ) {
                int humanInputLength{};

                char current1stDirKeypadPosition{ 'A' };
                for (char dirPadButtonRobot1 : possibleTransitionsRobot1) {
                    char current2ndDirKeypadPosition{ 'A' };
                    for (
                        char dirPadButtonRobot2 :
                            directionalKeypadTransitions.at(current1stDirKeypadPosition).at(dirPadButtonRobot1).front()
                    ) {
                        humanInputLength += directionalKeypadTransitions.at(current2ndDirKeypadPosition).at(dirPadButtonRobot2).front().length();

                        current2ndDirKeypadPosition = dirPadButtonRobot2;
                    }

                    current1stDirKeypadPosition = dirPadButtonRobot1;
                }

                if (humanInputLength < minSequenceLength) {
                    minSequenceLength = humanInputLength;
                }
            }

            sequenceLength += minSequenceLength;
            currentKeypadPosition = numPadButton;
        }

        line.pop_back();
        sumOfComplexities += sequenceLength * std::stoi(line);
    }

    return sumOfComplexities;
}


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    return 0;
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