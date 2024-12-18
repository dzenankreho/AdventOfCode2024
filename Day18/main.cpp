#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>


#define MY_INPUT_PATH R"(..\inputDay18.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day18.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day18.txt)"


struct Position {
    int x;
    int y;
    inline static int minX{};
    inline static int minY{};
    inline static int maxX{ std::numeric_limits<int>::max() };
    inline static int maxY{ std::numeric_limits<int>::max() };

    bool isValid() const {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
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

struct State {
    Position position;
    int numOfSteps;
};

template <>
struct std::greater<State> {
    bool operator()(const State& state1, const State& state2) const {
        return state1.numOfSteps > state2.numOfSteps;
    }
};


int solutionPart1(const char* inputPath, const Position& endPosition, int numOfFallenBytes) {
    std::ifstream input(inputPath);

    std::unordered_set<Position> fallenBytes;
    char comma;
    for (int i{}, x, y; i < numOfFallenBytes && (input >> x >> comma >> y); fallenBytes.emplace(x, y), ++i);

    Position::maxX = endPosition.x;
    Position::maxY = endPosition.y;

    std::priority_queue<State, std::vector<State>, std::greater<State>> unprocessedStates;
    std::unordered_map<Position, int> numOfSteps;

    Position startPosition{ 0, 0 };
    unprocessedStates.emplace(startPosition, 0);
    numOfSteps[startPosition] = 0;
    int minNumOfSteps{ -1 };

    while (!unprocessedStates.empty()) {
        State currentState{ unprocessedStates.top() };
        unprocessedStates.pop();

        if (currentState.position == endPosition) {
            minNumOfSteps = currentState.numOfSteps;
            break;
        }

        std::vector<Position> neighbours;
        for (
            const Position& increment : {
                Position{ -1,  0 },
                Position{  1,  0 },
                Position{  0, -1 },
                Position{  0,  1 }
            }
        ) {
            if (
                Position newPosition{ currentState.position + increment };
                newPosition.isValid() && !fallenBytes.contains(newPosition)
            ) {
                neighbours.push_back(newPosition);
            }
        }

        for (const Position& neighbour : neighbours) {
            if (
                int newNumOfSteps{ currentState.numOfSteps + 1 };
                !numOfSteps.contains(neighbour) || newNumOfSteps < numOfSteps[neighbour]
            ) {
                numOfSteps[neighbour] = newNumOfSteps;
                unprocessedStates.emplace(neighbour, newNumOfSteps);
            }
        }
    }

    return minNumOfSteps;
}


std::string solutionPart2(const char* inputPath, const Position& endPosition) {
    std::ifstream input(inputPath);

    std::vector<Position> fallingBytes;
    char comma;
    for (int x, y; input >> x >> comma >> y; fallingBytes.emplace_back(x, y));

    Position::maxX = endPosition.x;
    Position::maxY = endPosition.y;

    Position startPosition{ 0, 0 };
    std::string blockingByte;

    std::unordered_set<Position> fallenBytes{ fallingBytes.begin(), fallingBytes.end() };

    for (auto it{ fallingBytes.rbegin() }; it != fallingBytes.rend(); fallenBytes.erase(*it++)) {
        bool endPositionFound{ false };

        std::stack<Position> toVisit;
        toVisit.push(startPosition);

        std::unordered_set<Position> visited;

        while (!toVisit.empty()) {
            Position currentPosition{ toVisit.top() };
            toVisit.pop();

            if (currentPosition == endPosition) {
                endPositionFound = true;
                break;
            }

            if (visited.contains(currentPosition)) {
                continue;
            }

            visited.insert(currentPosition);

            for (
                const Position& increment : {
                    Position{ -1,  0 },
                    Position{  1,  0 },
                    Position{  0, -1 },
                    Position{  0,  1 }
                }
            ) {
                if (
                    Position newPosition{ currentPosition + increment };
                    newPosition.isValid() && !fallenBytes.contains(newPosition)
                ) {
                    toVisit.push(newPosition);
                }
            }
        }

        if (endPositionFound) {
            --it;
            blockingByte += std::to_string(it->x) + "," + std::to_string(it->y);
            break;
        }
    }

    return blockingByte;
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
    std::cout << "\tPart 1: " << solutionPart1(TEST_INPUT_PART1_PATH, {6, 6}, 12) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(TEST_INPUT_PART2_PATH, {6, 6}) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH, {70, 70}, 1024) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH, {70, 70}) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH, {70, 70}, 1024); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH, {70, 70}); }, 1000) << std::endl;

    return 0;
}
