#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>


#define MY_INPUT_PATH R"(..\inputDay17.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day17.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day17.txt)"


std::string solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<char, int> registers;
    char currentRegister = 'A';
    for (std::string line; std::getline(input, line) && !line.empty(); ++currentRegister) {
        registers[currentRegister] = std::stoi(line.substr(line.rfind(' ') + 1));
    }

    std::string programStr;
    input >> programStr;

    std::vector<int> program;
    for (int opcode; input >> opcode; ) {
        program.push_back(opcode);
        char comma;
        input >> comma;
    }

    std::string output;

    for (int instrPtr{}; instrPtr < program.size(); instrPtr += 2) {
        switch (program.at(instrPtr)) {
            case 0:
                if (program.at(instrPtr + 1) < 4) {
                    registers.at('A') /= 1 << program.at(instrPtr + 1);
                } else {
                    registers.at('A') /= 1 << registers.at('A' + program.at(instrPtr + 1) - 4);
                }
                break;

            case 1:
                registers.at('B') ^= program.at(instrPtr + 1);
                break;

            case 2:
                if (program.at(instrPtr + 1) < 4) {
                    registers.at('B') = program.at(instrPtr + 1) % 8;
                } else {
                    registers.at('B') = registers.at('A' + program.at(instrPtr + 1) - 4) % 8;
                }
                break;

            case 3:
                if (registers.at('A') != 0) {
                    instrPtr = program.at(instrPtr + 1) - 2;
                }
                break;

            case 4:
                registers.at('B') ^= registers.at('C');
                break;

            case 5:
                if (!output.empty()) {
                    output += ',';
                }
                if (program.at(instrPtr + 1) < 4) {
                    output += std::to_string(program.at(instrPtr + 1) % 8);
                } else {
                    output += std::to_string(registers.at('A' + program.at(instrPtr + 1) - 4) % 8);
                }
                break;

            case 6:
                if (program.at(instrPtr + 1) < 4) {
                    registers.at('B') = registers.at('A') / (1 << program.at(instrPtr + 1));
                } else {
                    registers.at('B') = registers.at('A') / (1 << registers.at('A' + program.at(instrPtr + 1) - 4));
                }
                break;

            case 7:
                if (program.at(instrPtr + 1) < 4) {
                    registers.at('C') = registers.at('A') / (1 << program.at(instrPtr + 1));
                } else {
                    registers.at('C') = registers.at('A') / (1 << registers.at('A' + program.at(instrPtr + 1) - 4));
                }
                break;
        }
    }

    return output;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<char, long long> registers;
    char currentRegister = 'A';
    for (std::string line; std::getline(input, line) && !line.empty(); ++currentRegister) {
        registers[currentRegister] = std::stoi(line.substr(line.rfind(' ') + 1));
    }

    std::string programStr;
    input >> programStr;

    std::vector<int> program;
    for (int opcode; input >> opcode; ) {
        program.push_back(opcode);
        char comma;
        input >> comma;
    }

    std::optional<long long> registerA{ std::nullopt };
    for (long long regAValue{ 1ll << (3 * (long long)(program.size() - 1)) }; !registerA; ++regAValue) {
        registers.at('A') = regAValue;
        int outPtr{};
        bool matching{ true };

        for (int instrPtr{}; instrPtr < program.size(); instrPtr += 2) {
            switch (program.at(instrPtr)) {
                case 0:
                    if (program.at(instrPtr + 1) < 4) {
                        registers.at('A') /= 1ll << program.at(instrPtr + 1);
                    } else {
                        registers.at('A') /= 1ll << registers.at('A' + program.at(instrPtr + 1) - 4);
                    }
                    break;

                case 1:
                    registers.at('B') ^= program.at(instrPtr + 1);
                    break;

                case 2:
                    if (program.at(instrPtr + 1) < 4) {
                        registers.at('B') = program.at(instrPtr + 1) % 8;
                    } else {
                        registers.at('B') = registers.at('A' + program.at(instrPtr + 1) - 4) % 8;
                    }
                    break;

                case 3:
                    if (registers.at('A') != 0) {
                        instrPtr = program.at(instrPtr + 1) - 2;
                    }
                    break;

                case 4:
                    registers.at('B') ^= registers.at('C');
                    break;

                case 5:
                    if (program.at(instrPtr + 1) < 4) {
                        if (
                            outPtr >= program.size() ||
                            program.at(outPtr) != program.at(instrPtr + 1) % 8
                        ) {
                            matching = false;
                        } else {
                            ++outPtr;
                        }
                    } else {
                        if (
                            outPtr >= program.size() ||
                            program.at(outPtr) != registers.at('A' + program.at(instrPtr + 1) - 4) % 8
                        ) {
                            matching = false;
                        } else {
                            ++outPtr;
                        }
                    }
                    break;

                case 6:
                    if (program.at(instrPtr + 1) < 4) {
                        registers.at('B') = registers.at('A') / (1ll << program.at(instrPtr + 1));
                    } else {
                        registers.at('B') = registers.at('A') / (1ll << registers.at('A' + program.at(instrPtr + 1) - 4));
                    }
                    break;

                case 7:
                    if (program.at(instrPtr + 1) < 4) {
                        registers.at('C') = registers.at('A') / (1ll << program.at(instrPtr + 1));
                    } else {
                        registers.at('C') = registers.at('A') / (1ll << registers.at('A' + program.at(instrPtr + 1) - 4));
                    }
                    break;
            }

            if (!matching) {
                break;
            }
        }

        if (outPtr == program.size()) {
            registerA = regAValue;
        }
    }

    return *registerA;
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
//    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
//    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}


// 2,4
// 1,5
// 7,5
// 1,6
// 0,3
// 4,0
// 5,5
// 3,0