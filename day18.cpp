#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include <fstream>
using namespace std;

const int GRID_SIZE = 71;
const vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

bool isValid(int x, int y, const vector<vector<char>>& grid, const set<pair<int, int>>& visited) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[y][x] == '.' && !visited.count({ x, y });
}

int findShortestPath(const vector<vector<char>>& grid, pair<int, int> start, pair<int, int> end) {
    queue<pair<pair<int, int>, int>> q;
    set<pair<int, int>> visited;
    q.push({ start, 0 });
    visited.insert(start);

    while (!q.empty()) {
        auto [current, steps] = q.front(); q.pop();
        if (current == end) return steps;

        for (const auto& [dx, dy] : directions) {
            int nx = current.first + dx, ny = current.second + dy;
            if (isValid(nx, ny, grid, visited)) {
                visited.insert({ nx, ny });
                q.push({ {nx, ny}, steps + 1 });
            }
        }
    }
    return -1;
}

int main() {
    ifstream inputFile("day18");
    vector<pair<int, int>> bytePositions;
    string line;

    while (getline(inputFile, line)) {
        int x, y; char comma;
        stringstream(line) >> x >> comma >> y;
        bytePositions.emplace_back(x, y);
    }

    vector<vector<char>> grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
    pair<int, int> start = { 0, 0 }, end = { 70, 70 };

    for (int i = 0; i < 1024; ++i)
        grid[bytePositions[i].second][bytePositions[i].first] = '#';

    int shortestPath = findShortestPath(grid, start, end);
    cout << "Part 1: " << (shortestPath != -1 ? "The shortest path takes " + to_string(shortestPath) + " steps." : "Error.") << endl;

    grid.assign(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
    for (size_t i = 0; i < bytePositions.size(); ++i) {
        auto [x, y] = bytePositions[i];
        grid[y][x] = '#';
        if (findShortestPath(grid, start, end) == -1) {
            cout << "Part 2: The first blocking byte is at " << x << "," << y << endl;
            return 0;
        }
    }

    cout << "Part 2: Error." << endl;
    return 0;
}
