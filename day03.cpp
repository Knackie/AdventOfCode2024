#include <iostream>
#include <fstream>
#include <string>
#include <regex>

int processMemory(const std::string& input, bool part2) {
    std::regex instructionRegex(R"(mul\(\d+,\d+\)|do\(\)|don't\(\))");
    std::sregex_iterator begin(input.begin(), input.end(), instructionRegex), end;

    bool mulEnabled = true; // Mul activé par défaut
    int resultSum = 0;

    for (auto it = begin; it != end; ++it) {
        std::string instr = it->str();
        if (instr == "do()") {
            mulEnabled = true;
        }
        else if (instr == "don't()") {
            mulEnabled = false;
        }
        else if (instr.find("mul") == 0 && (mulEnabled || !part2)) {
            std::regex numbersRegex(R"(\d+)");
            std::sregex_iterator numBegin(instr.begin(), instr.end(), numbersRegex);
            int x = std::stoi(numBegin->str()), y = std::stoi((++numBegin)->str());
            resultSum += x * y;
        }
    }

    return resultSum;
}

int main() {
    std::ifstream inputFile("day3.txt");
    if (!inputFile) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier input.txt" << std::endl;
        return 1;
    }

    std::string inputContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    std::cout << "Part 1: Sum of all multiplications: " << processMemory(inputContent, false) << std::endl;
    std::cout << "Part 2: Sum of enabled multiplications: " << processMemory(inputContent, true) << std::endl;

    return 0;
}
