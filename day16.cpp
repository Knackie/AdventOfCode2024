#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <tuple>
#include <algorithm>

using namespace std;

// Directions for movement: Up, Right, Down, Left
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
const int TURN_COST = 1000;

struct Node {
    int row, col, direction, cost;
    bool operator>(const Node& other) const { return cost > other.cost; }
};

int main() {
    // Hardcoded input file
    string file_name = "day16.txt";
    ifstream input_file(file_name);
    if (!input_file) {
        cerr << "Error: Could not open file " << file_name << endl;
        return 1;
    }

    vector<string> maze;
    string line;
    while (getline(input_file, line)) {
        maze.push_back(line);
    }
    input_file.close();

    int row_count = maze.size();
    int col_count = maze[0].size();
    pair<int, int> start_position, end_position;

    // Find the start ('S') and end ('E') positions
    for (int row = 0; row < row_count; ++row) {
        for (int col = 0; col < col_count; ++col) {
            if (maze[row][col] == 'S') start_position = { row, col };
            if (maze[row][col] == 'E') end_position = { row, col };
        }
    }

    // Priority queue for Dijkstra's algorithm
    priority_queue<Node, vector<Node>, greater<Node>> priority_queue;
    set<tuple<int, int, int>> visited_nodes;
    map<tuple<int, int, int>, int> distance_map;

    // Start processing from the start position
    priority_queue.push({ start_position.first, start_position.second, 1, 0 });
    int best_cost = -1;

    while (!priority_queue.empty()) {
        Node current = priority_queue.top();
        priority_queue.pop();

        if (distance_map.count({ current.row, current.col, current.direction }) &&
            distance_map[{current.row, current.col, current.direction}] <= current.cost) {
            continue;
        }
        distance_map[{current.row, current.col, current.direction}] = current.cost;

        if (current.row == end_position.first && current.col == end_position.second && best_cost == -1) {
            best_cost = current.cost;
        }

        if (visited_nodes.count({ current.row, current.col, current.direction })) {
            continue;
        }
        visited_nodes.insert({ current.row, current.col, current.direction });

        // Move forward
        int next_row = current.row + DIRECTIONS[current.direction][0];
        int next_col = current.col + DIRECTIONS[current.direction][1];
        if (next_row >= 0 && next_row < row_count && next_col >= 0 && next_col < col_count &&
            maze[next_row][next_col] != '#') {
            priority_queue.push({ next_row, next_col, current.direction, current.cost + 1 });
        }

        // Rotate left and right
        priority_queue.push({ current.row, current.col, (current.direction + 1) % 4, current.cost + TURN_COST });
        priority_queue.push({ current.row, current.col, (current.direction + 3) % 4, current.cost + TURN_COST });
    }

    cout << best_cost << endl;

    // Reverse traversal from the end position
    priority_queue = {};
    visited_nodes.clear();
    map<tuple<int, int, int>, int> reverse_distance_map;

    for (int direction = 0; direction < 4; ++direction) {
        priority_queue.push({ end_position.first, end_position.second, direction, 0 });
    }

    while (!priority_queue.empty()) {
        Node current = priority_queue.top();
        priority_queue.pop();

        if (reverse_distance_map.count({ current.row, current.col, current.direction }) &&
            reverse_distance_map[{current.row, current.col, current.direction}] <= current.cost) {
            continue;
        }
        reverse_distance_map[{current.row, current.col, current.direction}] = current.cost;

        if (visited_nodes.count({ current.row, current.col, current.direction })) {
            continue;
        }
        visited_nodes.insert({ current.row, current.col, current.direction });

        // Move backward
        int prev_row = current.row - DIRECTIONS[current.direction][0];
        int prev_col = current.col - DIRECTIONS[current.direction][1];
        if (prev_row >= 0 && prev_row < row_count && prev_col >= 0 && prev_col < col_count &&
            maze[prev_row][prev_col] != '#') {
            priority_queue.push({ prev_row, prev_col, current.direction, current.cost + 1 });
        }

        // Rotate left and right
        priority_queue.push({ current.row, current.col, (current.direction + 1) % 4, current.cost + TURN_COST });
        priority_queue.push({ current.row, current.col, (current.direction + 3) % 4, current.cost + TURN_COST });
    }

    // Determine the tiles part of an optimal path
    set<pair<int, int>> optimal_tiles;
    for (int row = 0; row < row_count; ++row) {
        for (int col = 0; col < col_count; ++col) {
            for (int direction = 0; direction < 4; ++direction) {
                if (distance_map.count({ row, col, direction }) && reverse_distance_map.count({ row, col, direction }) &&
                    distance_map[{row, col, direction}] + reverse_distance_map[{row, col, direction}] == best_cost) {
                    optimal_tiles.insert({ row, col });
                }
            }
        }
    }

    cout << optimal_tiles.size() << endl;

    return 0;
}
