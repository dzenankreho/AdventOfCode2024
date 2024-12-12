#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>


#define MY_INPUT_PATH R"(..\inputDay12.txt)"
#define TEST_INPUT1_PART1_PATH R"(..\testInput1Part1Day12.txt)"
#define TEST_INPUT2_PART1_PATH R"(..\testInput2Part1Day12.txt)"
#define TEST_INPUT3_PART1_PATH R"(..\testInput3Part1Day12.txt)"
#define TEST_INPUT1_PART2_PATH R"(..\testInput1Part2Day12.txt)"
#define TEST_INPUT2_PART2_PATH R"(..\testInput2Part2Day12.txt)"
#define TEST_INPUT3_PART2_PATH R"(..\testInput3Part2Day12.txt)"
#define TEST_INPUT4_PART2_PATH R"(..\testInput4Part2Day12.txt)"
#define TEST_INPUT5_PART2_PATH R"(..\testInput5Part2Day12.txt)"


struct IsValidPosition {
    inline static int minX{};
    inline static int minY{};
    inline static int maxX{ std::numeric_limits<int>::max() };
    inline static int maxY{ std::numeric_limits<int>::max() };

    static bool check(int x, int y) {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }
};


void findAllPlotsInRegion(
    const std::vector<std::string>& gardenMap,
    std::vector<std::vector<bool>>& visited,
    const char plantType,
    int& area,
    int& perimeter,
    const int x,
    const int y
) {
    if (
        !IsValidPosition::check(x, y) ||
        gardenMap.at(x).at(y) != plantType
    ) {
        perimeter++;
        return;
    }

    if (visited.at(x).at(y)) {
        return;
    }

    visited.at(x).at(y) = true;
    area++;

    findAllPlotsInRegion(gardenMap, visited, plantType, area, perimeter, x + 1, y);
    findAllPlotsInRegion(gardenMap, visited, plantType, area, perimeter, x - 1, y);
    findAllPlotsInRegion(gardenMap, visited, plantType, area, perimeter, x, y + 1);
    findAllPlotsInRegion(gardenMap, visited, plantType, area, perimeter, x, y - 1);
}


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> gardenMap;
    for (std::string line; std::getline(input, line); gardenMap.push_back(line));

    IsValidPosition::maxX = gardenMap.size() - 1;
    IsValidPosition::maxY = gardenMap.front().length() - 1;

    std::vector<std::vector<bool>> visited(
        IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
    );

    int totalPriceOfFencing{};
    for (int i{}; i < IsValidPosition::maxX + 1; ++i) {
        for (int j{}; j < IsValidPosition::maxY + 1; ++j) {
            if (visited.at(i).at(j)) {
                continue;
            }

            int area{}, perimeter{};
            findAllPlotsInRegion(gardenMap, visited, gardenMap.at(i).at(j), area, perimeter, i, j);
            totalPriceOfFencing += area * perimeter;
        }
    }

    return totalPriceOfFencing;
}


struct RegionBounds {
    int minX{ std::numeric_limits<int>::max() };
    int minY{ std::numeric_limits<int>::max() };
    int maxX{};
    int maxY{};
};


void findAllPlotsInRegion(
        const std::vector<std::string>& gardenMap,
        std::vector<std::vector<bool>>& visitedInGarden,
        std::vector<std::vector<bool>>& visitedInRegion,
        const char plantType,
        int& area,
        RegionBounds& regionBounds,
        const int x,
        const int y
) {
    if (
        !IsValidPosition::check(x, y) ||
        gardenMap.at(x).at(y) != plantType
    ) {
        regionBounds.minX = std::min(regionBounds.minX, x);
        regionBounds.maxX = std::max(regionBounds.maxX, x);
        regionBounds.minY = std::min(regionBounds.minY, y);
        regionBounds.maxY = std::max(regionBounds.maxY, y);

        return;
    }

    if (visitedInRegion.at(x).at(y)) {
        return;
    }

    visitedInGarden.at(x).at(y) = visitedInRegion.at(x).at(y) = true;
    area++;

    findAllPlotsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x + 1, y);
    findAllPlotsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x - 1, y);
    findAllPlotsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x, y + 1);
    findAllPlotsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x, y - 1);
}


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> gardenMap;
    for (std::string line; std::getline(input, line); gardenMap.push_back(line));

    IsValidPosition::maxX = gardenMap.size() - 1;
    IsValidPosition::maxY = gardenMap.front().length() - 1;

    std::vector<std::vector<bool>> visitedInGarden(
            IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
    );

    int totalPriceOfFencing{};
    for (int i{}; i < IsValidPosition::maxX + 1; ++i) {
        for (int j{}; j < IsValidPosition::maxY + 1; ++j) {
            if (visitedInGarden.at(i).at(j)) {
                continue;
            }

            int area{};
            RegionBounds regionBounds;
            std::vector<std::vector<bool>> visitedInRegion(
                    IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
            );
            findAllPlotsInRegion(
                gardenMap,
                visitedInGarden,
                visitedInRegion,
                gardenMap.at(i).at(j),
                area,
                regionBounds,
                i,
                j
            );

            int numOfSides{};
            for (int k{ regionBounds.minX }; k < regionBounds.maxX; ++k) {
                for (int l{ regionBounds.minY }; l < regionBounds.maxY; ++l) {
                    bool topLeft{
                        IsValidPosition::check(k, l) &&
                        visitedInRegion.at(k).at(l) &&
                        gardenMap.at(k).at(l) == gardenMap.at(i).at(j)
                    },
                    topRight{
                        IsValidPosition::check(k, l + 1) &&
                        visitedInRegion.at(k).at(l + 1) &&
                        gardenMap.at(k).at(l + 1) == gardenMap.at(i).at(j)
                    },
                    bottomLeft{
                        IsValidPosition::check(k + 1, l) &&
                        visitedInRegion.at(k + 1).at(l) &&
                        gardenMap.at(k + 1).at(l) == gardenMap.at(i).at(j)
                    },
                    bottomRight{
                        IsValidPosition::check(k + 1, l + 1) &&
                        visitedInRegion.at(k + 1).at(l + 1) &&
                        gardenMap.at(k + 1).at(l + 1) == gardenMap.at(i).at(j)
                    };

                    int numOfSamePlantsIn2x2Window{
                        topLeft + topRight + bottomLeft + bottomRight
                    };

                    numOfSides += numOfSamePlantsIn2x2Window % 2 == 1;

                    if (numOfSamePlantsIn2x2Window == 2) {
                        numOfSides += 2 * ((topLeft && bottomRight) || (bottomLeft && topRight));
                    }
                }
            }

            totalPriceOfFencing += area * numOfSides;
        }
    }

    return totalPriceOfFencing;
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
    std::cout << "\tPart 1 Input 1: " << solutionPart1(TEST_INPUT1_PART1_PATH) << std::endl;
    std::cout << "\tPart 1 Input 2: " << solutionPart1(TEST_INPUT2_PART1_PATH) << std::endl;
    std::cout << "\tPart 1 Input 3: " << solutionPart1(TEST_INPUT3_PART1_PATH) << std::endl;
    std::cout << "\tPart 2 Input 1: " << solutionPart2(TEST_INPUT1_PART2_PATH) << std::endl;
    std::cout << "\tPart 2 Input 2: " << solutionPart2(TEST_INPUT2_PART2_PATH) << std::endl;
    std::cout << "\tPart 2 Input 3: " << solutionPart2(TEST_INPUT3_PART2_PATH) << std::endl;
    std::cout << "\tPart 2 Input 4: " << solutionPart2(TEST_INPUT4_PART2_PATH) << std::endl;
    std::cout << "\tPart 2 Input 5: " << solutionPart2(TEST_INPUT5_PART2_PATH) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
