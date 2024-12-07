#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <set>


#define MY_INPUT_PATH R"(..\inputDay7.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day7.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day7.txt)"


enum Operator{ add, multiply, concatenation };

bool isEquationTrue(
    const long long result,
    const std::set<Operator> &operators,
    const std::vector<long long>::iterator& operandsBegin,
    const std::vector<long long>::iterator& operandsEnd,
    long long currentResult = -1
) {
    if (operandsBegin == operandsEnd) {
        return result == currentResult;
    }

    if (currentResult == -1) {
        return isEquationTrue(
            result,
            operators,
            operandsBegin + 1,
            operandsEnd,
            *operandsBegin
        );
    }

    if (
        operators.contains(Operator::add) &&
        isEquationTrue(
            result,
            operators,
            operandsBegin + 1,
            operandsEnd,
            currentResult + *operandsBegin
        )
    ) {
        return true;
    }

    if (
        operators.contains(Operator::multiply) &&
        isEquationTrue(
            result,
            operators,
            operandsBegin + 1,
            operandsEnd,
            currentResult * *operandsBegin
        )
    ) {
        return true;
    }

    if (operators.contains(Operator::concatenation)) {
        long long rightConcatOp{ *operandsBegin };
        do {
            currentResult *= 10;
            rightConcatOp /= 10;
        } while (rightConcatOp != 0);

        if (isEquationTrue(
                result,
                operators,
                operandsBegin + 1,
                operandsEnd,
                currentResult + *operandsBegin
            )
        ) {
            return true;
        }
    }

    return false;
}

long long solutionPart(const char* inputPath, const std::set<Operator> &operators) {
    std::ifstream input(inputPath);

    long long totalCalibrationResult{};

    for(std::string line; std::getline(input, line); ) {
        std::stringstream ss(line);

        long long result;
        char twoDots;
        ss >> result >> twoDots;

        std::vector<long long> operands;
        for (long long operand; ss >> operand; operands.push_back(operand));

        totalCalibrationResult += result * isEquationTrue(result, operators, operands.begin(), operands.end());
    }

    return totalCalibrationResult;
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
    static std::set<Operator> operatorsPart1{ Operator::add, Operator::multiply };
    static std::set<Operator> operatorsPart2{ Operator::add, Operator::multiply, Operator::concatenation };

    std::cout << "Test inputs:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart(TEST_INPUT_PART1_PATH, operatorsPart1) << std::endl;
    std::cout << "\tPart 2: " << solutionPart(TEST_INPUT_PART2_PATH, operatorsPart2) << std::endl;
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart(MY_INPUT_PATH, operatorsPart1) << std::endl;
    std::cout << "\tPart 2: " << solutionPart(MY_INPUT_PATH, operatorsPart2) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart(MY_INPUT_PATH, operatorsPart1); }, 100) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart(MY_INPUT_PATH, operatorsPart2); }, 1000) << std::endl;

    return 0;
}
