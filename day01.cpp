#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cmath>

int main() {
    std::ifstream file("input.txt");
    if (!file) return std::cerr << "Erreur : Fichier introuvable\n", 1;

    std::vector<int> left, right;
    int a, b;
    while (file >> a >> b) left.push_back(a), right.push_back(b);

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int totalDistance = 0, similarityScore = 0;
    std::unordered_map<int, int> countRight;
    for (int num : right) ++countRight[num];
    for (size_t i = 0; i < left.size(); ++i) {
        totalDistance += std::abs(left[i] - right[i]);
        similarityScore += left[i] * countRight[left[i]];
    }

    std::cout << "Distance totale : " << totalDistance << "\n";
    std::cout << "Score de similarité : " << similarityScore << "\n";
    return 0;
}
