#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>


#define MY_INPUT_PATH R"(..\inputDay8.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day8.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day8.txt)"


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

struct Antenna {
    Position position;
    char frequency;

    Antenna(int x, int y, char frequency) : position{x, y}, frequency{ frequency } {}

    Position getDistanceInfo(const Antenna& antenna) const {
        return { position.x - antenna.position.x, position.y - antenna.position.y };
    }
};


int solution(const char* inputPath, bool countResonantHarmonics) {
    std::ifstream input(inputPath);

    std::vector<std::string> antennaMap;
    std::vector<Antenna> antennas;
    for (std::string line; std::getline(input, line); antennaMap.push_back(line)) {
        for (int i{}; i < line.length(); ++i) {
            if (std::isalnum(line.at(i))) {
                antennas.emplace_back(antennaMap.size(), i, line.at(i));
            }
        }
    }

    Position::maxX = antennaMap.size() - 1;
    Position::maxY = antennaMap.front().size() - 1;

    std::unordered_set<Position> uniqueAntinodePositions;

    for (int i{}; i < antennas.size(); ++i) {
        for (int j{}; j < antennas.size(); ++j) {
            if (i != j && antennas.at(i).frequency == antennas.at(j).frequency) {
                int harmonic{ !countResonantHarmonics };
                do {
                    if (
                        Position antinodePosition{
                            antennas.at(i).position + (antennas.at(i).getDistanceInfo(antennas.at(j)) * harmonic)
                        }; antinodePosition.isValid()
                    ) {
                        uniqueAntinodePositions.insert(antinodePosition);
                    } else {
                        break;
                    }
                    ++harmonic;
                } while (countResonantHarmonics);
            }
        }
    }

    return uniqueAntinodePositions.size();
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
    std::cout << "\tPart 1: " << solution(TEST_INPUT_PART1_PATH, false) << std::endl;
    std::cout << "\tPart 2: " << solution(TEST_INPUT_PART2_PATH, true) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solution(MY_INPUT_PATH, false) << std::endl;
    std::cout << "\tPart 2: " << solution(MY_INPUT_PATH, true) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solution(MY_INPUT_PATH, false); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solution(MY_INPUT_PATH, true); }, 1000) << std::endl;

    return 0;
}
