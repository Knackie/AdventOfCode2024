#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include <fstream>
using namespace std;

// Grid size
const int GRID_SIZE = 71;

// Directions for moving (Up, Down, Left, Right)
const vector<pair<int, int>> directions = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

// Function to check if a position is valid and not corrupted
bool isValid(int x, int y, const vector<vector<char>> &grid, const set<pair<int, int>> &visited) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[y][x] == '.' && visited.find({x, y}) == visited.end();
}

// Function to find the shortest path using BFS
int findShortestPath(const vector<vector<char>> &grid, pair<int, int> start, pair<int, int> end) {
    queue<pair<pair<int, int>, int>> q; // (position, steps)
    set<pair<int, int>> visited;

    q.push({start, 0});
    visited.insert(start);

    while (!q.empty()) {
        auto [current, steps] = q.front();
        q.pop();

        int x = current.first;
        int y = current.second;

        if (current == end) {
            return steps;
        }

        for (const auto &[dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (isValid(nx, ny, grid, visited)) {
                visited.insert({nx, ny});
                q.push({{nx, ny}, steps + 1});
            }
        }
    }

    return -1; // No path found
}

int main() {
    // Read byte positions from file
    ifstream inputFile("day18");
    string line;
    vector<pair<int, int>> bytePositions;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        int x, y;
        char comma;
        ss >> x >> comma >> y;
        bytePositions.push_back({x, y});
    }

    // Initialize the grid
    vector<vector<char>> grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));

    // Part 1: Find the shortest path after 1024 bytes have fallen
    for (int i = 0; i < 1024; ++i) {
        int x = bytePositions[i].first;
        int y = bytePositions[i].second;
        grid[y][x] = '#';
    }

    pair<int, int> start = {0, 0};
    pair<int, int> end = {70, 70};

    int shortestPath = findShortestPath(grid, start, end);

    if (shortestPath != -1) {
        cout << "Part 1: The shortest path takes " << shortestPath << " steps." << endl;
    } else {
        cout << "Part 1: No path to the destination found." << endl;
    }

    // Part 2: Find the first byte that blocks the path completely
    grid.assign(GRID_SIZE, vector<char>(GRID_SIZE, '.'));

    for (size_t i = 0; i < bytePositions.size(); ++i) {
        int x = bytePositions[i].first;
        int y = bytePositions[i].second;
        grid[y][x] = '#';

        if (findShortestPath(grid, start, end) == -1) {
            cout << "Part 2: The first blocking byte is at " << x << "," << y << endl;
            return 0;
        }
    }

    cout << "Part 2: Path to the destination is always available." << endl;
    return 0;
}
