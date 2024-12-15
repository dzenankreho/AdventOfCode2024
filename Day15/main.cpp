#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>


#define MY_INPUT_PATH R"(..\inputDay15.txt)"
#define TEST_INPUT1_PART1_PATH R"(..\testInput1Part1Day15.txt)"
#define TEST_INPUT2_PART1_PATH R"(..\testInput2Part1Day15.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day15.txt)"


struct Position {
    int x;
    int y;

    Position& operator+=(const Position& position) {
        x += position.x;
        y += position.y;

        return *this;
    }

    Position operator+(const Position& position) const {
        return { x + position.x, y + position.y };
    }

    bool operator==(const Position& position) const {
        return x == position.x && y == position.y;
    }
};

template <>
struct std::hash<Position> {
    std::size_t operator()(const Position& position) const {
        return std::hash<int>()(position.x) ^ (std::hash<int>()(position.y) << 1);
    }
};


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_set<Position> boxes, walls;
    Position robot;
    int lineNumber{};
    for (std::string line; std::getline(input, line) && !line.empty(); ++lineNumber) {
        for (int i{}; i < line.length(); ++i) {
            if (line.at(i) == 'O') {
                boxes.emplace(lineNumber, i);
                continue;
            }

            if (line.at(i) == '#') {
                walls.emplace(lineNumber, i);
                continue;
            }

            if (line.at(i) == '@') {
                robot = { lineNumber, i };
                continue;
            }
        }
    }

    std::unordered_map<char, Position> moves {
        { '^', Position{ -1,  0 } },
        { 'v', Position{  1,  0 } },
        { '<', Position{  0, -1 } },
        { '>', Position{  0,  1 } }
    };

    for (std::string line; std::getline(input, line); ) {
        for (char move : line) {
            Position newRobotPosition{ robot + moves.at(move) };

            if (walls.contains(newRobotPosition)) {
                continue;
            }

            if (boxes.contains(newRobotPosition)) {
                bool freeSpaceFound{ false };
                for (
                    Position newBoxPosition{ newRobotPosition + moves.at(move) };
                    !walls.contains(newBoxPosition);
                    newBoxPosition += moves.at(move)
                ) {
                    if (!boxes.contains(newBoxPosition)) {
                        freeSpaceFound = true;
                        boxes.erase(newRobotPosition);
                        boxes.insert(newBoxPosition);
                        break;
                    }
                }

                if (!freeSpaceFound) {
                    continue;
                }
            }

            robot = newRobotPosition;
        }
    }

    long long sumOfBoxGpsCoordinates{};
    for (const Position& box : boxes) {
        sumOfBoxGpsCoordinates += box.x * 100 + box.y;
    }

    return sumOfBoxGpsCoordinates;
}


struct Box {
    Position leftPart;
    Position rightPart;

    bool operator==(const Box& box) const {
        return leftPart == box.leftPart || rightPart == box.rightPart ||
               leftPart == box.rightPart || rightPart == box.leftPart;
    }

    Box& operator+=(const Position& position) {
        leftPart += position;
        rightPart += position;

        return *this;
    }
};


bool moveBoxes(
    int initBoxToMoveIdx,
    const char move,
    const std::unordered_map<char, Position>& moves,
    const std::unordered_set<Position>& walls,
    std::vector<Box>& boxes
) {
    boxes.at(initBoxToMoveIdx) += moves.at(move);

    if (
        walls.contains(boxes.at(initBoxToMoveIdx).leftPart) ||
        walls.contains(boxes.at(initBoxToMoveIdx).rightPart)
    ) {
        return false;
    }

    for (int i{}; i < boxes.size(); ++i) {
        if (i == initBoxToMoveIdx) {
            continue;
        }

        if (
            boxes.at(i) == boxes.at(initBoxToMoveIdx) &&
            !moveBoxes(i, move, moves, walls, boxes)
        ) {
            return false;
        }
    }

    return true;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<Box> boxes;
    std::unordered_set<Position> walls;
    Position robot;
    int lineNumber{};
    for (std::string line; std::getline(input, line) && !line.empty(); ++lineNumber) {
        for (int i{}; i < line.length(); ++i) {
            if (line.at(i) == 'O') {
                line.at(i) = '[';
                line.insert(i + 1, 1, ']');
                ++i;
                continue;
            }

            if (line.at(i) == '#') {
                line.insert(i + 1, 1, '#');
                ++i;
                continue;
            }

            if (line.at(i) == '@' || line.at(i) == '.') {
                line.insert(i + 1, 1, '.');
                ++i;
                continue;
            }
        }

        for (int i{}; i < line.length(); ++i) {
            if (line.at(i) == '[') {
                boxes.emplace_back(Position{ lineNumber, i}, Position{ lineNumber, i + 1 });
                continue;
            }

            if (i != 0 && i + 1 != line.length() && line.at(i) == '#') {
                walls.emplace(lineNumber, i);
                continue;
            }

            if (line.at(i) == '@') {
                robot = { lineNumber, i };
                continue;
            }
        }
    }

    std::unordered_map<char, Position> moves {
        { '^', Position{ -1,  0 } },
        { 'v', Position{  1,  0 } },
        { '<', Position{  0, -1 } },
        { '>', Position{  0,  1 } }
    };

    for (std::string line; std::getline(input, line); ) {
        for (char move : line) {
            Position newRobotPosition{ robot + moves.at(move) };

            if (walls.contains(newRobotPosition)) {
                continue;
            }

            if (
                auto box{
                    std::find_if(
                        boxes.begin(),
                        boxes.end(),
                        [newRobotPosition](const Box &box) -> bool {
                            return box.leftPart == newRobotPosition || box.rightPart == newRobotPosition;
                        }
                    )
                }; box != boxes.end()
            ) {
                std::vector<Box> boxesCopy{ boxes };
                if (!moveBoxes(box - boxes.begin(), move, moves, walls, boxes)) {
                    boxes = boxesCopy;
                    continue;
                }
            }

            robot = newRobotPosition;
        }
    }

    long long sumOfBoxGpsCoordinates{};
    for (const Box& box : boxes) {
        sumOfBoxGpsCoordinates += box.leftPart.x * 100 + box.leftPart.y;
    }

    return sumOfBoxGpsCoordinates;
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
    std::cout << "\tInput 1 Part 1: " << solutionPart1(TEST_INPUT1_PART1_PATH) << std::endl;
    std::cout << "\tInput 2 Part 1: " << solutionPart1(TEST_INPUT2_PART1_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(TEST_INPUT_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
