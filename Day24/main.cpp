#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>


#define MY_INPUT_PATH R"(..\inputDay24.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day24.txt)"


struct LogicGate {
    enum class Type { AND, OR, XOR } type;

    std::string inputWire1;
    std::string inputWire2;
    std::string outputWire;

    void operator()(std::unordered_map<std::string, bool>& wireStates) const {
        if (
            wireStates.contains(outputWire) ||
            !wireStates.contains(inputWire1) ||
            !wireStates.contains(inputWire2)
        ) {
            return;
        }

        switch (type) {
            case Type::AND:
                wireStates.emplace(outputWire, wireStates.at(inputWire1) & wireStates.at(inputWire2));
                break;

            case Type::OR:
                wireStates.emplace(outputWire, wireStates.at(inputWire1) | wireStates.at(inputWire2));
                break;

            case Type::XOR:
                wireStates.emplace(outputWire, wireStates.at(inputWire1) ^ wireStates.at(inputWire2));
                break;
        }
    }

    bool areInputWires(const std::string& wire1, const std::string& wire2) const {
        return (inputWire1 == wire1 && inputWire2 == wire2) ||
               (inputWire1 == wire2 && inputWire2 == wire1);
    }

    bool operator==(const LogicGate& logicGate) const = default;
};

template <>
struct std::hash<LogicGate> {
    std::size_t operator()(const LogicGate& logicGate) const {
        std::size_t hashValue{ std::hash<LogicGate::Type>()(logicGate.type) };

        hashValue ^= std::hash<std::string>()(logicGate.inputWire1) << 1;
        hashValue ^= std::hash<std::string>()(logicGate.inputWire2) << 1;
        hashValue ^= std::hash<std::string>()(logicGate.outputWire) << 1;

        return hashValue;
    }
};


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<std::string, bool> wireStates;
    std::unordered_set<std::string> wires;

    for (std::string line; std::getline(input, line) && !line.empty(); ) {
        std::stringstream ss{ line };

        std::string wire;
        bool state;
        ss >> wire >> state;
        wire.pop_back();

        wires.insert(wire);
        wireStates.emplace(wire, state);
    }

    std::vector<LogicGate> logicGates;
    std::set<std::string> zWires;

    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss{ line };

        LogicGate logicGate;
        std::string logicGateType, arrow;
        ss >> logicGate.inputWire1 >> logicGateType >> logicGate.inputWire2 >> arrow >> logicGate.outputWire;

        wires.insert(logicGate.inputWire1);
        wires.insert(logicGate.inputWire2);
        wires.insert(logicGate.outputWire);

        if (logicGateType == "AND") {
            logicGate.type = LogicGate::Type::AND;
        } else if (logicGateType == "OR") {
            logicGate.type = LogicGate::Type::OR;
        } else if (logicGateType == "XOR") {
            logicGate.type = LogicGate::Type::XOR;
        }

        logicGates.push_back(logicGate);

        if (logicGate.outputWire.front() == 'z') {
            zWires.insert(logicGate.outputWire);
        }
    }

    while (wires.size() != wireStates.size()) {
        for (const LogicGate& logicGate : logicGates) {
            logicGate(wireStates);
        }
    }

    long long binaryNumber{}, i{};
    for (const std::string& zWire : zWires) {
        binaryNumber += wireStates.at(zWire) * (1ll << i++);
    }

    return binaryNumber;
}


std::string solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    for (std::string line; std::getline(input, line) && !line.empty(); );

    std::vector<LogicGate> logicGates;
    std::string zCarry{ "z00" };

    for (std::string line; std::getline(input, line); ) {
        std::stringstream ss{ line };

        LogicGate logicGate;
        std::string logicGateType, arrow;
        ss >> logicGate.inputWire1 >> logicGateType >> logicGate.inputWire2 >> arrow >> logicGate.outputWire;

        if (logicGateType == "AND") {
            logicGate.type = LogicGate::Type::AND;
        } else if (logicGateType == "OR") {
            logicGate.type = LogicGate::Type::OR;
        } else if (logicGateType == "XOR") {
            logicGate.type = LogicGate::Type::XOR;
        }

        logicGates.push_back(logicGate);

        if (logicGate.outputWire.front() == 'z' && logicGate.outputWire > zCarry) {
            zCarry = logicGate.outputWire;
        }
    }

    std::unordered_set<LogicGate> verifiedLogicGates;

    if (
        auto logicGateWithZ00Output{
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [](const LogicGate& logicGate) { return logicGate.outputWire == "z00"; }
            )
        };
        logicGateWithZ00Output->type == LogicGate::Type::XOR &&
        logicGateWithZ00Output->areInputWires("x00", "y00")
    ) {
        verifiedLogicGates.insert(*logicGateWithZ00Output);
    }

    if (
        auto logicGateWithZCarryOutput{
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [zCarry](const LogicGate& logicGate) { return logicGate.outputWire == zCarry; }
            )
        };
        logicGateWithZCarryOutput->type == LogicGate::Type::OR &&
        std::count_if(
                logicGates.begin(),
                logicGates.end(),
                [logicGateWithZCarryOutput](const LogicGate& lGate) {
                    return lGate.type == LogicGate::Type::AND &&
                           (logicGateWithZCarryOutput->inputWire1 == lGate.outputWire ||
                            logicGateWithZCarryOutput->inputWire2 == lGate.outputWire);
                }
        ) == 2
    ) {
        verifiedLogicGates.insert(*logicGateWithZCarryOutput);
    }

    if (
        auto logicGateWithZ01Output{
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [](const LogicGate& logicGate) { return logicGate.outputWire == "z01"; }
            )
        }, andLogicGateWithX00Y00Input{
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [](const LogicGate& logicGate) {
                    return logicGate.type == LogicGate::Type::AND && logicGate.areInputWires("x00", "y00");
                }
            )
        };
        logicGateWithZ01Output->type == LogicGate::Type::XOR &&
        (andLogicGateWithX00Y00Input->outputWire == logicGateWithZ01Output->inputWire1 ||
            andLogicGateWithX00Y00Input->outputWire == logicGateWithZ01Output->inputWire2)
    ) {
        verifiedLogicGates.insert(*logicGateWithZ01Output);
        verifiedLogicGates.insert(*andLogicGateWithX00Y00Input);
    }

    for (const LogicGate& logicGate : logicGates) {
        if (verifiedLogicGates.contains(logicGate)) {
            continue;
        }

        if (
            logicGate.type == LogicGate::Type::OR &&
            (std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [logicGate](const LogicGate& lGate) {
                    return lGate.type == LogicGate::Type::XOR &&
                           (logicGate.outputWire == lGate.inputWire1 ||
                                logicGate.outputWire == lGate.inputWire2) &&
                            lGate.outputWire.front() == 'z';
                }
            ) != logicGates.end() ||
            (std::count_if(
                logicGates.begin(),
                logicGates.end(),
                [logicGate](const LogicGate& lGate) {
                    return lGate.type == LogicGate::Type::AND &&
                           (logicGate.inputWire1 == lGate.outputWire ||
                                logicGate.inputWire2 == lGate.outputWire);
                }
            ) == 2 && logicGate.outputWire.front() != 'z'))
        ) {
            verifiedLogicGates.insert(logicGate);
            continue;
        }

        if (
            logicGate.type == LogicGate::Type::AND &&
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [logicGate](const LogicGate& lGate) {
                    return lGate.type == LogicGate::Type::OR &&
                           (logicGate.outputWire == lGate.inputWire1 ||
                                logicGate.outputWire == lGate.inputWire2);
                }
            ) != logicGates.end() &&
            std::find_if(
                logicGates.begin(),
                logicGates.end(),
                [logicGate](const LogicGate& lGate) {
                    return lGate.type == LogicGate::Type::XOR &&
                           logicGate.areInputWires(lGate.inputWire1, lGate.inputWire2);
                }
            ) != logicGates.end()
        ) {
            verifiedLogicGates.insert(logicGate);
            continue;
        }

        if (
            logicGate.type == LogicGate::Type::XOR &&
            ((logicGate.outputWire.front() == 'z' &&
                std::find_if(
                    logicGates.begin(),
                    logicGates.end(),
                    [logicGate](const LogicGate& lGate) {
                        return (lGate.type == LogicGate::Type::XOR || lGate.type == LogicGate::Type::OR) &&
                               (logicGate.inputWire1 == lGate.outputWire ||
                                    logicGate.inputWire2 == lGate.outputWire);
                    }
                ) != logicGates.end()) ||
             (((logicGate.inputWire1.front() == 'x') || (logicGate.inputWire2.front() == 'x')) &&
                std::find_if(
                    logicGates.begin(),
                    logicGates.end(),
                    [logicGate](const LogicGate& lGate) {
                        return lGate.type == LogicGate::Type::XOR &&
                               (logicGate.outputWire == lGate.inputWire1 ||
                                    logicGate.outputWire == lGate.inputWire2);
                    }
                ) != logicGates.end()))
        ) {
            verifiedLogicGates.insert(logicGate);
            continue;
        }
    }

    std::set<std::string> wiresInvolvedInSwaps;
    for (const LogicGate& logicGate : logicGates) {
        if (!verifiedLogicGates.contains(logicGate)) {
            wiresInvolvedInSwaps.insert(logicGate.outputWire);
        }
    }

    std::string wiresInvolvedInSwapsStr;
    for (const std::string& wire : wiresInvolvedInSwaps) {
        wiresInvolvedInSwapsStr += wire + ',';
    }
    wiresInvolvedInSwapsStr.pop_back();

    return wiresInvolvedInSwapsStr;
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
    std::cout << "My input:" << std::endl;
    std::cout << "\tPart 1: " << solutionPart1(MY_INPUT_PATH) << std::endl;
    std::cout << "\tPart 2: " << solutionPart2(MY_INPUT_PATH) << std::endl;
    std::cout << "My input runtime [ms]:" << std::endl;
    std::cout << "\tPart 1: " << measureTime([](){ solutionPart1(MY_INPUT_PATH); }, 1000) << std::endl;
    std::cout << "\tPart 2: " << measureTime([](){ solutionPart2(MY_INPUT_PATH); }, 1000) << std::endl;

    return 0;
}
