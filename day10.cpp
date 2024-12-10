#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

// Memoization for Part 2
map<pair<int, int>, int> memo;

// Function to check if a position is valid for traversal
bool isValid(int x, int y, int prevHeight, const vector<vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    return x >= 0 && x < rows && y >= 0 && y < cols &&
        map[x][y] == prevHeight - 1;  // Check for descending height
}

// Part 2: Count distinct paths to reach height 0
int dfsRating(int x, int y, const vector<vector<int>>& map) {
    if (map[x][y] == 0) {  // Base case: Reached height 0
        return 1;
    }
    if (memo.find({ x, y }) != memo.end()) {  // Check memoized result
        return memo[{x, y}];
    }

    int count = 0;
    vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (auto [dx, dy] : directions) {
        int nx = x + dx, ny = y + dy;
        if (isValid(nx, ny, map[x][y], map)) {
            count += dfsRating(nx, ny, map);
        }
    }
    memo[{x, y}] = count;  // Memoize result
    return count;
}

// Part 1: Count reachable trailheads (0) from height 9
int ways1(int x, int y, const vector<vector<int>>& map) {
    set<pair<int, int>> visited;
    set<pair<int, int>> reachableZeros;
    vector<pair<int, int>> stack = { {x, y} };

    while (!stack.empty()) {
        auto [cx, cy] = stack.back();
        stack.pop_back();
        if (visited.count({ cx, cy })) continue;
        visited.insert({ cx, cy });

        if (map[cx][cy] == 0) {
            reachableZeros.insert({ cx, cy });
        }

        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (auto [dx, dy] : directions) {
            int nx = cx + dx, ny = cy + dy;
            if (isValid(nx, ny, map[cx][cy], map)) {
                stack.push_back({ nx, ny });
            }
        }
    }

    return reachableZeros.size();
}

int main() {
    // Open the file
    ifstream inputFile("10.txt");
    if (!inputFile) {
        cerr << "Error: Could not open file 10.txt" << endl;
        return 1;
    }

    // Read the map data from the file
    vector<vector<int>> map;
    string line;
    while (getline(inputFile, line)) {
        vector<int> row;
        for (char c : line) {
            if (isdigit(c)) {
                row.push_back(c - '0');  // Convert character to integer
            }
        }
        if (!row.empty()) {
            map.push_back(row);
        }
    }
    inputFile.close();

    int rows = map.size();
    int cols = map[0].size();

    int totalScore = 0;  // Part 1: Total score
    int totalRating = 0; // Part 2: Total rating

    // Loop through each position to identify trailheads
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] == 9) {  // Start from height 9
                totalScore += ways1(r, c, map);
                totalRating += dfsRating(r, c, map);
            }
        }
    }

    // Output the results
    cout << "Total Score (Part 1): " << totalScore << endl;
    cout << "Total Rating (Part 2): " << totalRating << endl;

    return 0;
}
