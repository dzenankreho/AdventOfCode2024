#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>


#define MY_INPUT_PATH R"(..\inputDay23.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day23.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day23.txt)"


struct SetOfThreeComputers{
    std::unordered_set<std::string> computers;

    SetOfThreeComputers(
        const std::string& computer1,
        const std::string& computer2,
        const std::string& computer3
    ) {
        computers.insert(computer1);
        computers.insert(computer2);
        computers.insert(computer3);
    }

    bool operator==(const SetOfThreeComputers& set) const {
        for (const std::string& computer : set.computers) {
            if (!computers.contains(computer)) {
                return false;
            }
        }

        return true;
    }
};

template <>
struct std::hash<SetOfThreeComputers> {
    std::size_t operator()(const SetOfThreeComputers& set) const {
        std::size_t hashValue{};
        for (const std::string& computer : set.computers) {
            hashValue ^= std::hash<std::string>()(computer) << 1;
        }

        return hashValue;
    }
};


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<std::string, std::unordered_set<std::string>> computerNetwork;
    for (std::string line; std::getline(input, line); ) {
        std::string computer1{ line.substr(0, 2) }, computer2{ line.substr(3) };
        computerNetwork[computer1].insert(computer2);
        computerNetwork[computer2].insert(computer1);
    }

    std::unordered_set<SetOfThreeComputers> setsOfThreeComputersWithAtLeastOneStartingT;
    for (const auto& [ computer1, connectedComputers ] : computerNetwork) {
        if (computer1.front() != 't') {
            continue;
        }

        for (const std::string& computer2 : connectedComputers) {
            for (const std::string& computer3 : computerNetwork.at(computer2)) {
                if (computerNetwork.at(computer3).contains(computer1)) {
                    setsOfThreeComputersWithAtLeastOneStartingT.emplace(computer1, computer2, computer3);
                }
            }
        }
    }

    return setsOfThreeComputersWithAtLeastOneStartingT.size();
}


void findLargestSetOfInterconnectedComputers(
    const std::unordered_map<std::string, std::unordered_set<std::string>>& computerNetwork,
    const std::unordered_set<std::string>& interconnectedComputers,
    const std::unordered_set<std::string>& potentialNewComputers,
    const std::unordered_set<std::string>& excludedComputers,
    std::set<std::string>& largestSetOfInterconnectedComputers
) {
    if (potentialNewComputers.empty() && excludedComputers.empty()) {
        if (interconnectedComputers.size() > largestSetOfInterconnectedComputers.size()) {
            largestSetOfInterconnectedComputers =
                std::set<std::string>{ interconnectedComputers.begin(), interconnectedComputers.end() };
        }
        return;
    }

    std::unordered_set<std::string> potentialNewComputersCopy{ potentialNewComputers },
                                    excludedComputersCopy{ excludedComputers };
    for (const std::string& computer : potentialNewComputers) {
        std::unordered_set<std::string> newInterconnectedComputers{ interconnectedComputers };
        newInterconnectedComputers.insert(computer);

        std::unordered_set<std::string> connectedComputersInPotentialNewComputers,
                                        connectedComputersInExcludedComputers;

        for (const std::string& connectedComputer : computerNetwork.at(computer)) {
            if (potentialNewComputersCopy.contains(connectedComputer)) {
                connectedComputersInPotentialNewComputers.insert(connectedComputer);
            }
            if (excludedComputersCopy.contains(connectedComputer)) {
                connectedComputersInExcludedComputers.insert(connectedComputer);
            }
        }

        findLargestSetOfInterconnectedComputers(
            computerNetwork,
            newInterconnectedComputers,
            connectedComputersInPotentialNewComputers,
            connectedComputersInExcludedComputers,
            largestSetOfInterconnectedComputers
        );

        excludedComputersCopy.insert(computer);
        potentialNewComputersCopy.erase(computer);
    }
}


std::string solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::unordered_map<std::string, std::unordered_set<std::string>> computerNetwork;
    std::unordered_set<std::string> computersInNetwork;

    for (std::string line; std::getline(input, line); ) {
        std::string computer1{ line.substr(0, 2) }, computer2{ line.substr(3) };
        computerNetwork[computer1].insert(computer2);
        computerNetwork[computer2].insert(computer1);
        computersInNetwork.insert(computer1);
        computersInNetwork.insert(computer2);
    }

    std::set<std::string> largestSetOfInterconnectedComputers;
    findLargestSetOfInterconnectedComputers(
        computerNetwork,
        std::unordered_set<std::string>{},
        computersInNetwork,
        std::unordered_set<std::string>{},
        largestSetOfInterconnectedComputers
    );

    std::string largestSetOfInterconnectedComputersString;
    for (const std::string& computer : largestSetOfInterconnectedComputers) {
        if (!largestSetOfInterconnectedComputersString.empty()) {
            largestSetOfInterconnectedComputersString += ',';
        }
        largestSetOfInterconnectedComputersString += computer;
    }

    return largestSetOfInterconnectedComputersString;
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
