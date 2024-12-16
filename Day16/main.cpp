#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>


#define MY_INPUT_PATH R"(..\inputDay16.txt)"
#define TEST_INPUT1_PART1_PATH R"(..\testInput1Part1Day16.txt)"
#define TEST_INPUT2_PART1_PATH R"(..\testInput2Part1Day16.txt)"
#define TEST_INPUT1_PART2_PATH R"(..\testInput1Part2Day16.txt)"
#define TEST_INPUT2_PART2_PATH R"(..\testInput2Part2Day16.txt)"


struct Position {
    int x;
    int y;

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

enum class Direction{ east = 0, north = 1, west = 2, south = 3 };

inline bool areOpposite(const Direction& direction1, const Direction& direction2) {
    return Direction((int(direction1) + 2) % 4) == direction2;
}

struct Node {
    Position position;
    Direction direction;

    bool operator==(const Node& node) const {
        return position == node.position && direction == node.direction;
    }
};

template <>
struct std::hash<Node> {
    std::size_t operator()(const Node& node) const {
        return std::hash<Position>()(node.position) ^ (std::hash<Direction>()(node.direction) << 1);
    }
};

struct State {
    Node node;
    long long score;
};

template <>
struct std::greater<State> {
    bool operator()(const State& state1, const State& state2) const {
        return state1.score > state2.score;
    }
};


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> map;
    Position startPosition, endPosition;
    for (std::string line; std::getline(input, line); map.push_back(line)) {
        if (std::size_t found{ line.find('S') }; found != std::string::npos) {
            startPosition.x = map.size();
            startPosition.y = found;
        }

        if (std::size_t found{ line.find('E') }; found != std::string::npos) {
            endPosition.x = map.size();
            endPosition.y = found;
        }
    }

    std::priority_queue<State, std::vector<State>, std::greater<State>> unprocessedStates;
    std::unordered_map<Node, long long> scores;

    Node startNode{ startPosition, Direction::east };
    unprocessedStates.emplace(startNode, 0);
    scores[startNode] = 0;
    long long minScore{ -1 };

    while (!unprocessedStates.empty()) {
        State currentState{ unprocessedStates.top() };
        unprocessedStates.pop();

        if (currentState.node.position == endPosition) {
            minScore = currentState.score;
            break;
        }

        std::vector<Node> neighbours;
        for (
            auto& [increment, direction] : {
                Node{ Position{ -1,  0 }, Direction::north },
                Node{ Position{  1,  0 }, Direction::south },
                Node{ Position{  0, -1 }, Direction::west  },
                Node{ Position{  0,  1 }, Direction::east  }
            }
        ) {
            if (
                Position newPosition{ currentState.node.position + increment };
                map.at(newPosition.x).at(newPosition.y) != '#' &&
                    !areOpposite(currentState.node.direction, direction)
            ) {
                neighbours.emplace_back(newPosition, direction);
            }
        }

        for (const Node& neighbour : neighbours) {
            if (
                long long newScore{
                    currentState.score + 1 + 1000 * (currentState.node.direction != neighbour.direction)
                }; !scores.contains(neighbour) || newScore < scores[neighbour]
            ) {
                scores[neighbour] = newScore;
                unprocessedStates.emplace(neighbour, newScore);
            }
        }
    }

    return minScore;
}


void findAllUniquePositionsInBestPaths(
    const Position& startPosition,
    const Node& currentNode,
    const std::unordered_map<Node, std::vector<Node>>& predecessors,
    std::unordered_set<Position>& positionsOnBestPaths
){
    positionsOnBestPaths.insert(currentNode.position);

    if (currentNode.position == startPosition) {
        return;
    }

    for (const Node& node : predecessors.at(currentNode)) {
        findAllUniquePositionsInBestPaths(startPosition, node, predecessors, positionsOnBestPaths);
    }
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> map;
    Position startPosition, endPosition;
    for (std::string line; std::getline(input, line); map.push_back(line)) {
        if (std::size_t found{ line.find('S') }; found != std::string::npos) {
            startPosition.x = map.size();
            startPosition.y = found;
        }

        if (std::size_t found{ line.find('E') }; found != std::string::npos) {
            endPosition.x = map.size();
            endPosition.y = found;
        }
    }

    std::priority_queue<State, std::vector<State>, std::greater<State>> unprocessedStates;
    std::unordered_map<Node, long long> scores;
    std::unordered_map<Node, std::vector<Node>> predecessors;

    Node startNode{ startPosition, Direction::east };
    unprocessedStates.emplace(startNode, 0);
    scores[startNode] = 0;

    std::unordered_set<Position> positionsOnBestPaths;

    while (!unprocessedStates.empty()) {
        State currentState{ unprocessedStates.top() };
        unprocessedStates.pop();

        if (currentState.node.position == endPosition) {
            findAllUniquePositionsInBestPaths(startPosition, currentState.node, predecessors, positionsOnBestPaths);
            break;
        }

        std::vector<Node> neighbours;
        for (
            auto& [increment, direction] : {
                Node{ Position{ -1,  0 }, Direction::north },
                Node{ Position{  1,  0 }, Direction::south },
                Node{ Position{  0, -1 }, Direction::west  },
                Node{ Position{  0,  1 }, Direction::east  }
            }
        ) {
            if (
                Position newPosition{ currentState.node.position + increment };
                map.at(newPosition.x).at(newPosition.y) != '#' &&
                    !areOpposite(currentState.node.direction, direction)
            ) {
                neighbours.emplace_back(newPosition, direction);
            }
        }

        for (const Node& neighbour : neighbours) {
            if (
                long long newScore{
                    currentState.score + 1 + 1000 * (currentState.node.direction != neighbour.direction)
                }; !scores.contains(neighbour) || newScore < scores[neighbour]
            ) {
                scores[neighbour] = newScore;
                unprocessedStates.emplace(neighbour, newScore);
                predecessors[neighbour].clear();
                predecessors[neighbour].push_back(currentState.node);
            } else if (newScore == scores[neighbour]) {
                predecessors[neighbour].push_back(currentState.node);
            }
        }
    }

    return positionsOnBestPaths.size();
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
    std::cout << "\tInput 1 Part 2: " << solutionPart2(TEST_INPUT1_PART2_PATH) << std::endl;
    std::cout << "\tInput 2 Part 2: " << solutionPart2(TEST_INPUT2_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
