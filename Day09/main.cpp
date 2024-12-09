#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <string>
#include <list>


#define MY_INPUT_PATH R"(..\inputDay9.txt)"
#define TEST_INPUT_PART1_PATH R"(..\testInputPart1Day9.txt)"
#define TEST_INPUT_PART2_PATH R"(..\testInputPart2Day9.txt)"


struct DiskBlock {
    enum class Type{ file = 0, freeSpace = 1 } type;

    int size;
    int id;
};


long long solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::string diskMap;
    std::getline(input, diskMap);

    std::list<DiskBlock> blocks;
    DiskBlock::Type type{ DiskBlock::Type::file };
    int fileId{};

    for (char c : diskMap) {
        DiskBlock block { type, c - '0' };
        if (type == DiskBlock::Type::file) {
            block.id = fileId++;
        }

        if (block.size != 0) {
            blocks.push_back(block);
        }

        type = DiskBlock::Type(!bool(type));
    }


    for (auto fileBlock{ blocks.end() }, freeSpaceBlock{ blocks.begin() }; ;) {
        while (freeSpaceBlock != blocks.end() && (freeSpaceBlock)->type == DiskBlock::Type::file) {
            freeSpaceBlock++;
        }

        if (freeSpaceBlock == blocks.end()) {
            break;
        }

        while (fileBlock != blocks.begin() &&
                (fileBlock == blocks.end() || fileBlock->type == DiskBlock::Type::freeSpace)) {
            fileBlock--;
        }

        if (fileBlock == blocks.begin()) {
            break;
        }

        if (fileBlock->size == freeSpaceBlock->size) {
            *freeSpaceBlock = *fileBlock;
            fileBlock = blocks.erase(fileBlock);
            continue;
        }

        if (fileBlock->size < freeSpaceBlock->size) {
            freeSpaceBlock->size -= fileBlock->size;
            blocks.insert(freeSpaceBlock, *fileBlock);
            fileBlock = blocks.erase(fileBlock);
            continue;
        }

        if (fileBlock->size > freeSpaceBlock->size) {
            fileBlock->size -= freeSpaceBlock->size;
            freeSpaceBlock->id = fileBlock->id;
            freeSpaceBlock->type = DiskBlock::Type::file;
            continue;
        }
    }

    long long position{}, checksum{};
    for (auto file : blocks) {
        for (int i{}; i < file.size; ++i) {
            checksum += file.id * position++;
        }
    }

    return checksum;
}


long long solutionPart2(const char* inputPath) {
    std::ifstream input(inputPath);

    std::string diskMap;
    std::getline(input, diskMap);

    std::list<DiskBlock> blocks;
    DiskBlock::Type type{ DiskBlock::Type::file };
    int fileId{};

    for (char c : diskMap) {
        DiskBlock block { type, c - '0' };
        if (type == DiskBlock::Type::file) {
            block.id = fileId++;
        }

        if (block.size != 0) {
            blocks.push_back(block);
        }

        type = DiskBlock::Type(!bool(type));
    }

    int lastProcessedId{ fileId };
    for (auto fileBlock{ blocks.end() }; ; lastProcessedId = (fileBlock--)->id) {
        while (fileBlock != blocks.begin() &&
                (fileBlock == blocks.end() || fileBlock->type == DiskBlock::Type::freeSpace)) {
            fileBlock--;
        }

        if (fileBlock == blocks.begin()) {
            break;
        }

        if (fileBlock->id >= lastProcessedId) {
            continue;
        }

        if (
            auto freeSpaceBlock{
                std::find_if(
                    blocks.begin(),
                    fileBlock,
                    [fileBlock] (const DiskBlock& block) {
                        return block.type == DiskBlock::Type::freeSpace && fileBlock->size <= block.size;
                    }
                )
            }; freeSpaceBlock != fileBlock
        ) {
            freeSpaceBlock->size -= fileBlock->size;
            blocks.insert(freeSpaceBlock, *fileBlock);
            fileBlock->type = DiskBlock::Type::freeSpace;
        }
    }

    long long position{}, checksum{};
    for (auto block : blocks) {
        if (block.type == DiskBlock::Type::freeSpace) {
            position += block.size;
            continue;
        }

        for (int i{}; i < block.size; ++i) {
            checksum += block.id * position++;
        }
    }

    return checksum;
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
