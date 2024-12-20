#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>


#define MY_INPUT_PATH R"(..\inputDay20.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day20.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day20.txt)"


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


int solutionPart1(const char* inputPath, int minPicosecondsSaved) {
    std::ifstream input(inputPath);

    std::vector<std::string> racetrack;
    Position start, end;
    for (std::string line; std::getline(input, line); racetrack.push_back(line)) {
        if (std::size_t found{ line.find('S') }; found != std::string::npos) {
            start.x = racetrack.size();
            start.y = found;
        }

        if (std::size_t found{ line.find('E') }; found != std::string::npos) {
            end.x = racetrack.size();
            end.y = found;
        }
    }

    std::unordered_map<Position, int> picosecondsAtEachPosition;
    Position currentPosition{ start };

    std::vector<Position> positionIncrements {
        Position{ -1,  0 },
        Position{  1,  0 },
        Position{  0, -1 },
        Position{  0,  1 }
    };

    for (int picoseconds{}; ; ++picoseconds) {
        picosecondsAtEachPosition.emplace(currentPosition, picoseconds);

        if (currentPosition == end) {
            break;
        }

        for (const Position& increment : positionIncrements) {
            if (
                Position newPosition{ currentPosition + increment };
                !picosecondsAtEachPosition.contains(newPosition) &&
                    racetrack.at(newPosition.x).at(newPosition.y) != '#'
            ) {
                currentPosition = newPosition;
                break;
            }
        }
    }

    int numOfCheatsThatSaveMoreThanMinPicoseconds{};
    for (const auto& position : picosecondsAtEachPosition) {
        for (const Position& increment : positionIncrements) {
            if (
                Position firstCheatPosition{ position.first + increment },
                         secondCheatPosition{ firstCheatPosition + increment };
                !picosecondsAtEachPosition.contains(firstCheatPosition) &&
                picosecondsAtEachPosition.contains(secondCheatPosition) &&
                    picosecondsAtEachPosition.at(secondCheatPosition) -
                    picosecondsAtEachPosition.at(position.first) > minPicosecondsSaved
            ) {
                numOfCheatsThatSaveMoreThanMinPicoseconds++;
            }
        }
    }

    return numOfCheatsThatSaveMoreThanMinPicoseconds;
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
    std::cout << "\tPart 1: " << solutionPart1(TEST_INPUT_PART1_PATH, 0) << std::endl;
//    std::cout << "\tPart 2: " << solutionPart2(TEST_INPUT_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH, 100) << std::endl;
//    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH, 100); }, 1000) << std::endl;
//    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
