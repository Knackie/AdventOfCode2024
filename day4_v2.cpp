#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

// Directions pour la recherche de "XMAS"
const vector<pair<int, int>> directions = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

// Vérifie si une position est valide dans la grille
inline bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Prétraitement de la grille pour indexer les positions des caractères
unordered_map<char, vector<pair<int, int>>> preprocessGrid(const vector<string>& grid) {
    unordered_map<char, vector<pair<int, int>>> charPositions;
    for (int x = 0; x < grid.size(); ++x) {
        for (int y = 0; y < grid[0].size(); ++y) {
            charPositions[grid[x][y]].emplace_back(x, y);
        }
    }
    return charPositions;
}

// Recherche de "XMAS" (Partie 1)
int countXMAS(const vector<string>& grid, const unordered_map<char, vector<pair<int, int>>>& positions) {
    int count = 0;
    for (const auto& [x, y] : positions.at('X')) { // Parcourt uniquement les positions des 'X'
        for (const auto& dir : directions) {
            if (isValid(x + 3 * dir.first, y + 3 * dir.second, grid.size(), grid[0].size()) &&
                grid[x + dir.first][y + dir.second] == 'M' &&
                grid[x + 2 * dir.first][y + 2 * dir.second] == 'A' &&
                grid[x + 3 * dir.first][y + 3 * dir.second] == 'S') {
                ++count;
            }
        }
    }
    return count;
}

// Vérifie un motif "X-MAS" (Partie 2)
int countXMASPatterns(const vector<string>& grid, const unordered_map<char, vector<pair<int, int>>>& positions) {
    int count = 0;
    for (const auto& [x, y] : positions.at('A')) { // Le centre d'un "X-MAS" doit être un 'A'
        if (isValid(x - 1, y - 1, grid.size(), grid[0].size()) &&
            isValid(x + 1, y + 1, grid.size(), grid[0].size()) &&
            isValid(x + 1, y - 1, grid.size(), grid[0].size()) &&
            isValid(x - 1, y + 1, grid.size(), grid[0].size())) {
            string topLeft = {grid[x - 1][y - 1], grid[x][y], grid[x + 1][y + 1]};
            string bottomLeft = {grid[x + 1][y - 1], grid[x][y], grid[x - 1][y + 1]};
            if ((topLeft == "MAS" || topLeft == "SAM") && (bottomLeft == "MAS" || bottomLeft == "SAM")) {
                ++count;
            }
        }
    }
    return count;
}

// Lecture de la grille depuis un fichier
vector<string> readGrid(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
        exit(1);
    }
    vector<string> grid;
    string line;
    while (getline(file, line)) grid.push_back(line);
    return grid;
}

int main() {
    vector<string> grid = readGrid("day4.txt");

    // Prétraitement des positions des caractères
    auto positions = preprocessGrid(grid);

    // Partie 1 : Recherche des occurrences du mot "XMAS"
    int totalXMAS = countXMAS(grid, positions);
    cout << "Partie 1 : Le mot 'XMAS' apparait " << totalXMAS << " fois dans la grille." << endl;

    // Partie 2 : Recherche des motifs "X-MAS"
    int totalPatterns = countXMASPatterns(grid, positions);
    cout << "Partie 2 : Le motif 'X-MAS' apparait " << totalPatterns << " fois dans la grille." << endl;

    return 0;
}
