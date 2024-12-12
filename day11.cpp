#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Fonction pour exécuter les transformations de clignement
std::vector<int> blink(std::vector<int> stones, int num_blinks) {
    for (int i = 0; i < num_blinks; ++i) {
        std::vector<int> new_stones;
        for (int stone : stones) {
            if (stone == 0) {
                new_stones.push_back(1);
            } else {
                std::string str_stone = std::to_string(stone);
                if (str_stone.length() % 2 == 0) { // Nombre pair de chiffres
                    int half = str_stone.length() / 2;
                    int left = std::stoi(str_stone.substr(0, half));
                    int right = std::stoi(str_stone.substr(half));
                    new_stones.push_back(left);
                    new_stones.push_back(right);
                } else { // Nombre impair de chiffres
                    new_stones.push_back(stone * 2024);
                }
            }
        }
        stones = new_stones;
    }
    return stones;
}

// Fonction pour lire les données initiales depuis un fichier
std::vector<int> readStonesFromFile(const std::string& filename) {
    std::vector<int> stones;
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << "\n";
        return stones;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream stream(line);
        int stone;
        while (stream >> stone) {
            stones.push_back(stone);
        }
    }

    inputFile.close();
    return stones;
}

int main() {
    // Lire les pierres initiales depuis le fichier "11.txt"
    const std::string filename = "11.txt";
    std::vector<int> stones = readStonesFromFile(filename);

    if (stones.empty()) {
        std::cerr << "Erreur : aucune pierre valide trouvée dans le fichier.\n";
        return 1;
    }

    // Calculer les pierres après 25 clignements
    int num_blinks_25 = 25;
    std::vector<int> result_25 = blink(stones, num_blinks_25);
    std::cout << "Nombre de pierres après 25 clignements : " << result_25.size() << std::endl;

    // Calculer les pierres après 75 clignements
    int num_blinks_75 = 75;
    std::vector<int> result_75 = blink(stones, num_blinks_75);
    std::cout << "Nombre de pierres après 75 clignements : " << result_75.size() << std::endl;

    return 0;
}
