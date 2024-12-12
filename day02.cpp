#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

bool isSafe(const std::vector<int>& report) {
    if (report.size() < 2) return true;
    bool increasing = true, decreasing = true;
    for (int i = 0; i < report.size() - 1; ++i) {
        int diff = report[i + 1] - report[i];
        if (std::abs(diff) < 1 || std::abs(diff) > 3) return false;
        if (diff > 0) decreasing = false;
        if (diff < 0) increasing = false;
    }
    return increasing || decreasing;
}

bool isSafeWithDampener(const std::vector<int>& report) {
    if (isSafe(report)) return true;
    for (int i = 0; i < report.size(); ++i) {
        auto modified = report;
        modified.erase(modified.begin() + i);
        if (isSafe(modified)) return true;
    }
    return false;
}

int main() {
    std::ifstream file("day2");
    if (!file) return std::cerr << "Erreur: Fichier introuvable\n", 1;

    int safeReportsPart1 = 0, safeReportsPart2 = 0;
    for (std::string line; std::getline(file, line);) {
        std::vector<int> report;
        for (std::istringstream ss(line); ss >> line;) report.push_back(std::stoi(line));
        safeReportsPart1 += isSafe(report);
        safeReportsPart2 += isSafeWithDampener(report);
    }

    std::cout << "Partie 1 - Nombre de rapports sécurisés : " << safeReportsPart1 << '\n';
    std::cout << "Partie 2 - Nombre de rapports sécurisés avec Problem Dampener : " << safeReportsPart2 << '\n';
    return 0;
}
