#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

// Directions for neighbor traversal (up, right, down, left)
const vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Check if a position is within bounds
bool inBounds(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Hash function for pair
struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

// Perform BFS to calculate area, perimeter, and sides
void exploreRegion(const vector<string>& grid, vector<vector<bool>>& visited, int startX, int startY, int& area, int& perimeter, int& sides) {
    char plantType = grid[startX][startY];
    queue<pair<int, int>> q;
    q.push({startX, startY});
    visited[startX][startY] = true;

    area = 0;
    perimeter = 0;
    unordered_map<pair<int, int>, unordered_set<pair<int, int>, PairHash>, PairHash> perimMap;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        area++;

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (!inBounds(nx, ny, grid.size(), grid[0].size()) || grid[nx][ny] != plantType) {
                perimeter++;
                perimMap[{dx, dy}].insert({x, y});
            } else if (!visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    sides = 0;
    for (auto& [direction, edgeSet] : perimMap) {
        unordered_set<pair<int, int>, PairHash> seenEdges;

        for (auto& edge : edgeSet) {
            if (seenEdges.count(edge) == 0) {
                sides++;
                queue<pair<int, int>> edgeQueue;
                edgeQueue.push(edge);

                while (!edgeQueue.empty()) {
                    auto [ex, ey] = edgeQueue.front();
                    edgeQueue.pop();

                    if (seenEdges.count({ex, ey})) continue;
                    seenEdges.insert({ex, ey});

                    for (const auto& [dx, dy] : directions) {
                        int enx = ex + dx;
                        int eny = ey + dy;
                        if (edgeSet.count({enx, eny})) {
                            edgeQueue.push({enx, eny});
                        }
                    }
                }
            }
        }
    }
}

// Calculate costs for fencing all regions
pair<int, int> calculateCosts(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    int totalCostPart1 = 0;
    int totalCostPart2 = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                int area = 0, perimeter = 0, sides = 0;
                exploreRegion(grid, visited, i, j, area, perimeter, sides);

                totalCostPart1 += area * perimeter;
                totalCostPart2 += area * sides;
            }
        }
    }

    return {totalCostPart1, totalCostPart2};
}

int main() {
    vector<string> grid;
    string line;
    ifstream inputFile("day12.txt");

    if (!inputFile) {
        cerr << "Error: Could not open the file 'day12.txt'." << endl;
        return 1;
    }

    while (getline(inputFile, line)) {
        grid.push_back(line);
    }

    inputFile.close();

    auto [totalCostPart1, totalCostPart2] = calculateCosts(grid);

    cout << "Part 1: Total cost = " << totalCostPart1 << endl;
    cout << "Part 2: Total cost = " << totalCostPart2 << endl;

    return 0;
}
