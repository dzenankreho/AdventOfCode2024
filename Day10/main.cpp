#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>


#define MY_INPUT_PATH R"(..\inputDay10.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day10.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day10.txt)"


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

    Position operator*(int num) const {
        return { x * num, y * num };
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


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> topographicMap;
    std::vector<Position> trailheads;
    for (std::string line; std::getline(input, line); topographicMap.push_back(line)) {
        for (int i{}; i < line.length(); ++i) {
            if (line.at(i) == '0') {
                trailheads.emplace_back(topographicMap.size(), i);
            }
        }
    }

    Position::maxX = topographicMap.size() - 1;
    Position::maxY = topographicMap.front().length() - 1;

    int sumOfTrailheadScores{};

    for (const Position& trailhead : trailheads) {
        std::stack<Position> toVisit;
        toVisit.push(trailhead);

        std::unordered_set<Position> visited, trailEnds;

        while (!toVisit.empty()) {
            Position currentPosition{ toVisit.top() };
            toVisit.pop();

            if (
                topographicMap.at(currentPosition.x).at(currentPosition.y) == '9' &&
                !trailEnds.contains(currentPosition)
            ) {
                sumOfTrailheadScores++;
                trailEnds.insert(currentPosition);
            }

            if (visited.contains(currentPosition)) {
                continue;
            }

            visited.insert(currentPosition);

            for (
                const Position& increment : {
                    Position(-1, 0), Position(1, 0), Position(0, -1), Position(0, 1)
                }
            ) {
                if (
                    Position newPosition{ currentPosition + increment };
                    newPosition.isValid() &&
                        topographicMap.at(currentPosition.x).at(currentPosition.y) + 1 ==
                        topographicMap.at(newPosition.x).at(newPosition.y)
                ) {
                    toVisit.push(newPosition);
                }
            }
        }
    }

    return sumOfTrailheadScores;
}


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> topographicMap;
    std::vector<Position> trailheads;
    for (std::string line; std::getline(input, line); topographicMap.push_back(line)) {
        for (int i{}; i < line.length(); ++i) {
            if (line.at(i) == '0') {
                trailheads.emplace_back(topographicMap.size(), i);
            }
        }
    }

    Position::maxX = topographicMap.size() - 1;
    Position::maxY = topographicMap.front().length() - 1;

    int sumOfTrailheadScores{};

    for (const Position& trailhead : trailheads) {
        std::stack<Position> toVisit;
        toVisit.push(trailhead);

        while (!toVisit.empty()) {
            Position currentPosition{ toVisit.top() };
            toVisit.pop();

            sumOfTrailheadScores += topographicMap.at(currentPosition.x).at(currentPosition.y) == '9';

            for (
                const Position& increment : {
                    Position(-1, 0), Position(1, 0), Position(0, -1), Position(0, 1)
                }
            ) {
                if (
                    Position newPosition{ currentPosition + increment };
                    newPosition.isValid() &&
                        topographicMap.at(currentPosition.x).at(currentPosition.y) + 1 ==
                        topographicMap.at(newPosition.x).at(newPosition.y)
                ) {
                    toVisit.push(newPosition);
                }
            }
        }
    }

    return sumOfTrailheadScores;
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
