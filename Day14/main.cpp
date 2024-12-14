#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <cstdio>
#include <vector>
#include <optional>


#define MY_INPUT_PATH R"(..\inputDay14.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day14.txt)"


struct Vec2D {
    int x;
    int y;

    Vec2D operator*(int num) const {
        return { x * num, y * num };
    }

    Vec2D& operator+=(const Vec2D& vec2D)  {
        x += vec2D.x;
        y += vec2D.y;
        return *this;
    }

    Vec2D& operator%=(const Vec2D& modulo)  {
        x %= modulo.x;
        y %= modulo.y;

        if (x < 0) {
            x += modulo.x;
        }

        if (y < 0) {
            y += modulo.y;
        }

        return *this;
    }

    double squareDistance(const Vec2D& vec2D) const {
        return (x - vec2D.x) * (x - vec2D.x) + (y - vec2D.y) * (y - vec2D.y);
    }
};


struct Robot {
    Vec2D currentPosition;
    const Vec2D velocity;
};


int solutionPart1(const char* inputPath, const Vec2D& spaceSize) {
    std::ifstream input(inputPath);

    std::vector<Robot> robots;
    for (std::string line; std::getline(input, line); ) {
        int positionX, positionY, velocityX, velocityY;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &positionX, &positionY, &velocityX, &velocityY);

        robots.emplace_back(Vec2D{ positionX, positionY }, Vec2D{ velocityX, velocityY });
    }

    int numOfSeconds{ 100 };

    int numOfRobotsInFirstQuadrant{},
        numOfRobotsInSecondQuadrant{},
        numOfRobotsInThirdQuadrant{},
        numOfRobotsInFourthQuadrant{};

    for (Robot& robot : robots) {
        robot.currentPosition += robot.velocity * numOfSeconds;
        robot.currentPosition %= spaceSize;

        numOfRobotsInFirstQuadrant += (robot.currentPosition.x < spaceSize.x / 2) &&
                                      (robot.currentPosition.y < spaceSize.y / 2);

        numOfRobotsInSecondQuadrant += (robot.currentPosition.x > spaceSize.x / 2) &&
                                       (robot.currentPosition.y < spaceSize.y / 2);

        numOfRobotsInThirdQuadrant += (robot.currentPosition.x < spaceSize.x / 2) &&
                                      (robot.currentPosition.y > spaceSize.y / 2);

        numOfRobotsInFourthQuadrant += (robot.currentPosition.x > spaceSize.x / 2) &&
                                       (robot.currentPosition.y > spaceSize.y / 2);
    }

    return numOfRobotsInFirstQuadrant *
           numOfRobotsInSecondQuadrant *
           numOfRobotsInThirdQuadrant *
           numOfRobotsInFourthQuadrant;
}


int solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<Robot> robots;
    for (std::string line; std::getline(input, line); ) {
        int positionX, positionY, velocityX, velocityY;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &positionX, &positionY, &velocityX, &velocityY);

        robots.emplace_back(Vec2D{ positionX, positionY }, Vec2D{ velocityX, velocityY });
    }

    Vec2D spaceSize{ 101, 103 };

    double averageOfAverageSquareDistances{},
           christmasTreeDetectionThreshold{ 0.5 };
    std::optional<int> numOfSecondsToChristmasTree{ std::nullopt };

    for (int numOfSeconds{}; !numOfSecondsToChristmasTree; ++numOfSeconds) {
        for (Robot& robot : robots) {
            robot.currentPosition += robot.velocity;
            robot.currentPosition %= spaceSize;
        }

        double averageSquareDistanceBetweenRobots{};
        for (int i{}; i < robots.size(); ++i) {
            for (int j{ i + 1 }; j < robots.size(); ++j) {
                averageSquareDistanceBetweenRobots +=
                    robots.at(i).currentPosition.squareDistance(robots.at(j).currentPosition);
            }
        }
        averageSquareDistanceBetweenRobots /= (robots.size() * (robots.size() - 1)) / 2.0;

        if (
            numOfSeconds != 0 &&
            averageSquareDistanceBetweenRobots / averageOfAverageSquareDistances < christmasTreeDetectionThreshold
        ) {
            numOfSecondsToChristmasTree = numOfSeconds + 1;
        }

        if (numOfSeconds == 0) {
            averageOfAverageSquareDistances = averageSquareDistanceBetweenRobots;
        } else {
            averageOfAverageSquareDistances +=
                (averageSquareDistanceBetweenRobots - averageOfAverageSquareDistances) / (numOfSeconds + 1);
        }
    }

    return *numOfSecondsToChristmasTree;
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
    std::cout << "\tPart 1: " << solutionPart1(TEST_INPUT_PART1_PATH, { 11, 7 }) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH, { 101, 103 }) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH, { 101, 103 }); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 100) << std::endl;

    return 0;
}
