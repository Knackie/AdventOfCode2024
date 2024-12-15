#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <deque>
#include <tuple>
#include <algorithm>

using namespace std;

// Directions for exploring adjacent cells (up, right, down, left)
const int DIRS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

// Function to parse the input and extract integers
vector<int> parseLine(const string& line) {
    vector<int> result;
    stringstream ss(line);
    string temp;
    while (getline(ss, temp, ',')) {
        result.push_back(stoi(temp));
    }
    return result;
}

// Function to print the grid
void printGrid(const vector<vector<char>>& grid) {
    for (const auto& row : grid) {
        for (char c : row) {
            cout << c;
        }
        cout << '\n';
    }
}

int main() {
    const int X = 101; // Width of the grid
    const int Y = 103; // Height of the grid

    // Input data
    vector<tuple<int, int, int, int>> robots; // (px, py, vx, vy)
    string line;

    // Open file day14.txt
    ifstream infile("day14.txt");
    if (!infile) {
        cerr << "Error: Could not open file 'day14.txt'" << endl;
        return 1;
    }

    // Reading input
    while (getline(infile, line)) {
        stringstream ss(line);
        int px, py, vx, vy;
        char ignore;
        ss >> ignore >> ignore >> px >> ignore >> py >> ignore >> ignore >> ignore >> vx >> ignore >> vy;
        robots.emplace_back(px, py, vx, vy);
    }
    infile.close();

    int part1Result = 0;
    int alignmentTime = 0;

    for (int t = 1; t <= 1000000; ++t) {
        vector<vector<char>> grid(Y, vector<char>(X, '.'));

        int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
        int mx = X / 2, my = Y / 2;

        for (auto& [px, py, vx, vy] : robots) {
            px = (px + vx) % X;
            py = (py + vy) % Y;

            if (px < 0) px += X;
            if (py < 0) py += Y;

            grid[py][px] = '#';

            if (t == 100) {
                if (px < mx && py < my) ++q1;
                if (px >= mx && py < my) ++q2;
                if (px < mx && py >= my) ++q3;
                if (px >= mx && py >= my) ++q4;
            }
        }

        if (t == 100) {
            part1Result = q1 * q2 * q3 * q4;
            cout << "Part 1 Result: " << part1Result << endl;
        }

        // Count connected components for part 2
        vector<vector<bool>> visited(Y, vector<bool>(X, false));
        int components = 0;

        for (int y = 0; y < Y; ++y) {
            for (int x = 0; x < X; ++x) {
                if (grid[y][x] == '#' && !visited[y][x]) {
                    ++components;

                    // BFS to mark all cells in this component
                    deque<pair<int, int>> q;
                    q.emplace_back(x, y);

                    while (!q.empty()) {
                        auto [cx, cy] = q.front();
                        q.pop_front();

                        if (visited[cy][cx]) continue;
                        visited[cy][cx] = true;

                        for (const auto& dir : DIRS) {
                            int nx = cx + dir[0];
                            int ny = cy + dir[1];

                            if (nx >= 0 && nx < X && ny >= 0 && ny < Y && grid[ny][nx] == '#' && !visited[ny][nx]) {
                                q.emplace_back(nx, ny);
                            }
                        }
                    }
                }
            }
        }

        if (components <= 200) {
            alignmentTime = t;
            cout << "Part 2 Alignment Time: " << alignmentTime << endl;
            printGrid(grid);
            break;
        }
    }

    return 0;
}
