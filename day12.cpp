#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Direction vectors for moving up, down, left, and right
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Helper function for BFS to find regions
pair<int, int> bfs(const vector<string>& garden_map, vector<vector<bool>>& visited, int r, int c, char plant_type) {
    int rows = garden_map.size();
    int cols = garden_map[0].size();
    queue<pair<int, int>> q;
    q.push({r, c});
    visited[r][c] = true;

    int area = 0;
    int perimeter = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        area++;

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                if (garden_map[nx][ny] == plant_type && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                } else if (garden_map[nx][ny] != plant_type) {
                    perimeter++;
                }
            } else {
                perimeter++; // Edge of the map adds to the perimeter
            }
        }
    }

    return {area, perimeter};
}

int main() {
    // Read the garden map from the file "day12.cpp"
    ifstream file("day12.cpp");
    vector<string> garden_map;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            garden_map.push_back(line);
        }
    }
    file.close();

    int rows = garden_map.size();
    int cols = garden_map[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    unordered_map<char, int> region_costs;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c]) {
                char plant_type = garden_map[r][c];
                auto [area, perimeter] = bfs(garden_map, visited, r, c, plant_type);
                region_costs[plant_type] += area * perimeter;
            }
        }
    }

    // Calculate the total cost
    int total_cost = 0;
    for (const auto& [plant_type, cost] : region_costs) {
        total_cost += cost;
    }

    cout << "Total cost: " << total_cost << endl;
    return 0;
}
