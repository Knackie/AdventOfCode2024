#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

// Helper function to calculate Manhattan distance
inline int manhattan_distance(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}

// Optimized function to solve the problem
pair<int, int> solve(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    // Read the grid from the file
    vector<string> grid;
    string line;
    while (getline(file, line)) {
        grid.push_back(line);
    }
    file.close();

    int numRows = grid.size();
    int numCols = grid[0].size();

    // Store positions of non-dot characters
    map<char, vector<pair<int, int>>> characterPositions;
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (grid[row][col] != '.') {
                characterPositions[grid[row][col]].emplace_back(row, col);
            }
        }
    }

    set<pair<int, int>> part1Points;
    set<pair<int, int>> part2Points;

    // Process each character group
    for (const auto& [character, positions] : characterPositions) {
        int numPositions = positions.size();
        for (int i = 0; i < numPositions; ++i) {
            int row1 = positions[i].first, col1 = positions[i].second;
            for (int j = i + 1; j < numPositions; ++j) {
                int row2 = positions[j].first, col2 = positions[j].second;

                // Vector differences
                int deltaRow = row2 - row1;
                int deltaCol = col2 - col1;

                // Check all points collinear with (row1, col1) and (row2, col2)
                for (int row = max(0, min(row1, row2) - 1); row <= min(numRows - 1, max(row1, row2) + 1); ++row) {
                    for (int col = max(0, min(col1, col2) - 1); col <= min(numCols - 1, max(col1, col2) + 1); ++col) {
                        if ((row == row1 && col == col1) || (row == row2 && col == col2)) continue;

                        int deltaRow1 = row - row1, deltaCol1 = col - col1;
                        int deltaRow2 = row - row2, deltaCol2 = col - col2;

                        // Check if collinear
                        if (deltaRow1 * deltaCol2 == deltaCol1 * deltaRow2) {
                            part2Points.insert({row, col});

                            // Check the distance condition for part1Points
                            int distance1 = manhattan_distance(row, col, row1, col1);
                            int distance2 = manhattan_distance(row, col, row2, col2);
                            if (distance1 == 2 * distance2 || 2 * distance1 == distance2) {
                                part1Points.insert({row, col});
                            }
                        }
                    }
                }
            }
        }
    }

    // Results
    return {part1Points.size(), part2Points.size()};
}

int main() {
    string filename = "8.txt";  // Explicitly use "8.txt"
    auto [part1Result, part2Result] = solve(filename);

    cout << "Part 1: " << part1Result << endl;
    cout << "Part 2: " << part2Result << endl;

    return 0;
}
