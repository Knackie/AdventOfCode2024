#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
using namespace std;

// Function to check if a position is valid for traversal
bool isValid(int x, int y, int prevHeight, const vector<vector<int>>& map, const set<pair<int, int>>& visited) {
    int rows = map.size();
    int cols = map[0].size();
    return x >= 0 && x < rows && y >= 0 && y < cols &&
           visited.find({x, y}) == visited.end() &&
           map[x][y] == prevHeight + 1;
}

// Part 1: Depth-first search to calculate trailhead scores (count reachable 9s)
void dfsScore(int x, int y, const vector<vector<int>>& map, set<pair<int, int>>& visited, set<pair<int, int>>& reachableNines) {
    if (map[x][y] == 9) {
        reachableNines.insert({x, y});
        return;
    }
    visited.insert({x, y});
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto [dx, dy] : directions) {
        int nx = x + dx, ny = y + dy;
        if (isValid(nx, ny, map[x][y], map, visited)) {
            dfsScore(nx, ny, map, visited, reachableNines);
        }
    }
    visited.erase({x, y});
}

// Part 2: Depth-first search to calculate trailhead ratings (count distinct trails)
int dfsRating(int x, int y, const vector<vector<int>>& map, set<pair<int, int>>& visited) {
    visited.insert({x, y});
    int count = 0;
    bool isTrailEnd = true; // A trail ends if no valid moves are possible
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto [dx, dy] : directions) {
        int nx = x + dx, ny = y + dy;
        if (isValid(nx, ny, map[x][y], map, visited)) {
            isTrailEnd = false;
            count += dfsRating(nx, ny, map, visited);
        }
    }
    visited.erase({x, y});
    return count + (isTrailEnd ? 1 : 0); // Count as a trail if it's an end
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
                row.push_back(c - '0'); // Convert character to integer
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
            if (map[r][c] == 0) { // Trailhead found
                // Part 1: Compute trailhead score
                set<pair<int, int>> visited;
                set<pair<int, int>> reachableNines;
                dfsScore(r, c, map, visited, reachableNines);
                totalScore += reachableNines.size();

                // Part 2: Compute trailhead rating
                visited.clear();
                totalRating += dfsRating(r, c, map, visited);
            }
        }
    }

    // Output the results
    cout << "Total Score (Part 1): " << totalScore << endl;
    cout << "Total Rating (Part 2): " << totalRating << endl;

    return 0;
}
