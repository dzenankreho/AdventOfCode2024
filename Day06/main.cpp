#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <limits>
#include <unordered_set>
#include <thread>


#define MY_INPUT_PATH R"(..\inputDay6.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day6.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day6.txt)"


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

enum class Direction{ up = 0, right = 1, down = 2, left = 3 };


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> map;
    std::unordered_set<Position> guardPositions;
    Position lastGuardPosition;
    for (std::string line; std::getline(input, line); map.push_back(line)) {
        if (auto found{ line.find('^') }; found != std::string::npos) {
            lastGuardPosition.x = map.size();
            lastGuardPosition.y = found;
            guardPositions.insert(lastGuardPosition);
        }
    }

    Position::maxX = map.size() - 1;
    Position::maxY = map.front().length() - 1;

    Direction guardDirection{ Direction::up };

    for (Position newPosition; ; ) {
        switch (guardDirection) {
            case Direction::up:
                newPosition = lastGuardPosition + Position(-1, 0);
                break;

            case Direction::right:
                newPosition = lastGuardPosition + Position(0, 1);
                break;

            case Direction::down:
                newPosition = lastGuardPosition + Position(1, 0);
                break;

            case Direction::left:
                newPosition = lastGuardPosition + Position(0, -1);
                break;
        }

        if (!newPosition.isValid()) {
            break;
        }

        if (map.at(newPosition.x).at(newPosition.y) != '#') {
            guardPositions.insert(newPosition);
            lastGuardPosition = newPosition;
            continue;
        }

        guardDirection = Direction((int(guardDirection) + 1) % 4);
    }

    return guardPositions.size();
}


struct State {
    Position position;
    Direction direction;

    bool operator==(const State& state) const {
        return position == state.position && direction == state.direction;
    }
};

template <>
struct std::hash<State> {
    std::size_t operator()(const State& state) const {
        std::size_t positionHash = std::hash<Position>()(state.position);
        std::size_t directionHash = std::hash<int>()(static_cast<int>(state.direction));
        return positionHash ^ (directionHash << 1);
    }
};


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> map;
    std::unordered_set<Position> guardPositions;
    Position startGuardPosition;
    for (std::string line; std::getline(input, line); map.push_back(line)) {
        if (auto found{ line.find('^') }; found != std::string::npos) {
            startGuardPosition.x = map.size();
            startGuardPosition.y = found;
        }
    }

    Position::maxX = map.size() - 1;
    Position::maxY = map.front().length() - 1;

    Direction guardDirection{ Direction::up };

    for (Position lastGuardPosition{ startGuardPosition }, newPosition; ; ) {
        switch (guardDirection) {
            case Direction::up:
                newPosition = lastGuardPosition + Position(-1, 0);
                break;

            case Direction::right:
                newPosition = lastGuardPosition + Position(0, 1);
                break;

            case Direction::down:
                newPosition = lastGuardPosition + Position(1, 0);
                break;

            case Direction::left:
                newPosition = lastGuardPosition + Position(0, -1);
                break;
        }

        if (!newPosition.isValid()) {
            break;
        }

        if (map.at(newPosition.x).at(newPosition.y) != '#') {
            guardPositions.insert(newPosition);
            lastGuardPosition = newPosition;
            continue;
        }

        guardDirection = Direction((int(guardDirection) + 1) % 4);
    }

    std::size_t numOfInitGuardPositions{ guardPositions.size() };
    std::atomic<int> numOfPotentialLoopObstructions{};
    std::vector<std::thread> threads;
    threads.reserve(numOfInitGuardPositions);

    for (const Position& potentialObstruction : guardPositions) {
        threads.emplace_back(
            [
             &numOfPotentialLoopObstructions,
             &potentialObstruction,
             &startGuardPosition,
             &numOfInitGuardPositions
            ] (std::vector<std::string> map) -> void {
                map.at(potentialObstruction.x).at(potentialObstruction.y) = '#';

                State startGuardState{ startGuardPosition, Direction::up };
                std::unordered_set<State> guardStates;
                guardStates.reserve(numOfInitGuardPositions);

                bool directionChange{ false };
                for (State lastGuardState{ startGuardState }, newState; ; ) {
                    newState.direction = lastGuardState.direction;
                    switch (lastGuardState.direction) {
                        case Direction::up:
                            newState.position = lastGuardState.position + Position(-1, 0);
                            break;

                        case Direction::right:
                            newState.position = lastGuardState.position + Position(0, 1);
                            break;

                        case Direction::down:
                            newState.position = lastGuardState.position + Position(1, 0);
                            break;

                        case Direction::left:
                            newState.position = lastGuardState.position + Position(0, -1);
                            break;
                    }

                    if (directionChange && guardStates.contains(newState)) {
                        numOfPotentialLoopObstructions++;
                        break;
                    }

                    if (!newState.position.isValid()) {
                        break;
                    }

                    if (map.at(newState.position.x).at(newState.position.y) != '#') {
                        guardStates.insert(newState);
                        lastGuardState = newState;
                        directionChange = false;
                        continue;
                    }

                    lastGuardState.direction = Direction((int(lastGuardState.direction) + 1) % 4);
                    directionChange = true;
                }
            },
            map
        );
    }

    for (auto&& th : threads) {
        th.join();
    }

    return numOfPotentialLoopObstructions;
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
