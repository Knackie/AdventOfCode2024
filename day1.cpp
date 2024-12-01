#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <unordered_map>

// Fonction pour afficher une liste
void printList(const std::string& label, const std::vector<int>& list) {
    std::cout << label << ": ";
    for (int num : list) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// Fonction pour calculer la distance totale
int calculateTotalDistance(std::vector<int> left, std::vector<int> right) {
    // Trier les deux listes
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());


    int totalDistance = 0;

    // Calculer la distance totale en sommant les différences absolues
    for (size_t i = 0; i < left.size(); ++i) {
        totalDistance += std::abs(left[i] - right[i]);
    }

    return totalDistance;
}

// Fonction pour calculer le score de similarité
int calculateSimilarityScore(const std::vector<int>& left, const std::vector<int>& right) {
    // Compter les occurrences dans la liste de droite
    std::unordered_map<int, int> rightCount;
    for (int num : right) {
        rightCount[num]++;
    }

    // Calculer le score de similarité
    int similarityScore = 0;
    for (int num : left) {
        if (rightCount.find(num) != rightCount.end()) {
            similarityScore += num * rightCount[num];
        }
    }

    return similarityScore;
}

// Fonction pour lire les données depuis un fichier
void loadDataFromFile(const std::string& filename, std::vector<int>& left, std::vector<int>& right) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int leftNum, rightNum;
        if (iss >> leftNum >> rightNum) {
            left.push_back(leftNum);
            right.push_back(rightNum);
        }
    }

    file.close();

}

int main() {
    // Chemin du fichier
    std::string filename = "day1.txt"; // Remplacez par le chemin exact du fichier

    // Vecteurs pour les deux listes
    std::vector<int> left;
    std::vector<int> right;

    // Charger les données depuis le fichier
    loadDataFromFile(filename, left, right);

    // Vérifier si les deux listes ont la même taille
    if (left.size() != right.size()) {
        std::cerr << "Erreur : Les listes ne sont pas de la même taille." << std::endl;
        return EXIT_FAILURE;
    }

    // Calculer la distance totale
    int totalDistance = calculateTotalDistance(left, right);
    std::cout << "La distance totale est : " << totalDistance << std::endl;

    // Calculer le score de similarité
    int similarityScore = calculateSimilarityScore(left, right);
    std::cout << "Le score de similarité est : " << similarityScore << std::endl;

    return 0;
}
