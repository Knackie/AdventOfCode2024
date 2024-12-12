#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const vector<pair<int, int>> directions = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

// position exist
inline bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// P1
bool searchWord(const vector<string>& grid, int x, int y, const string& word, const pair<int, int>& dir) {
    for (int i = 0; i < word.size(); ++i) {
        int nx = x + i * dir.first, ny = y + i * dir.second;
        if (!isValid(nx, ny, grid.size(), grid[0].size()) || grid[nx][ny] != word[i]) return false;
    }
    return true;
}

// P2
bool isXMAS(const vector<string>& grid, int x, int y) {
    string MAS = "MAS", revMAS = "SAM";
    return isValid(x + 2, y + 2, grid.size(), grid[0].size()) &&
        ((grid[x][y] == MAS[0] && grid[x + 1][y + 1] == MAS[1] && grid[x + 2][y + 2] == MAS[2]) ||
            (grid[x][y] == revMAS[0] && grid[x + 1][y + 1] == revMAS[1] && grid[x + 2][y + 2] == revMAS[2])) &&
        ((grid[x + 2][y] == MAS[0] && grid[x + 1][y + 1] == MAS[1] && grid[x][y + 2] == MAS[2]) ||
            (grid[x + 2][y] == revMAS[0] && grid[x + 1][y + 1] == revMAS[1] && grid[x][y + 2] == revMAS[2]));
}

int countOccurrences(const vector<string>& grid, const string& word = "") {
    int count = 0;
    for (int x = 0; x < grid.size(); ++x) {
        for (int y = 0; y < grid[0].size(); ++y) {
            if (word.empty()) count += isXMAS(grid, x, y); // P2
            else for (const auto& dir : directions) count += searchWord(grid, x, y, word, dir); // P1
        }
    }
    return count;
}

vector<string> readGrid(const string& filename) {
    ifstream file(filename);
    if (!file) { cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl; exit(1); }
    vector<string> grid; string line;
    while (getline(file, line)) grid.push_back(line);
    return grid;
}

int main() {
    vector<string> grid = readGrid("day4");

    cout << "P1 : 'XMAS' " << countOccurrences(grid, "XMAS") << endl;
    cout << "P2 : 'X-MAS' " << countOccurrences(grid) << endl;

    return 0;
}
