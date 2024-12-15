#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <set>
#include <fstream>
#include <cassert>

using namespace std;

const vector<pair<int, int>> direction_offsets = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} }; // Up, Right, Down, Left

int calculate_sum(vector<string> grid, const string& instructions, bool expand_grid) {
    int row_count = grid.size();
    int col_count = grid[0].size();

    // Expand the grid if expand_grid is true
    if (expand_grid) {
        vector<string> expanded_grid;
        for (int row_index = 0; row_index < row_count; ++row_index) {
            string new_row;
            for (int col_index = 0; col_index < col_count; ++col_index) {
                if (grid[row_index][col_index] == '#') {
                    new_row += "##";
                }
                else if (grid[row_index][col_index] == 'O') {
                    new_row += "[]";
                }
                else if (grid[row_index][col_index] == '.') {
                    new_row += "..";
                }
                else if (grid[row_index][col_index] == '@') {
                    new_row += "@.";
                }
            }
            expanded_grid.push_back(new_row);
        }
        grid = expanded_grid;
        col_count *= 2;
    }

    // Locate the initial position of the robot
    int robot_row, robot_col;
    for (int row_index = 0; row_index < row_count; ++row_index) {
        for (int col_index = 0; col_index < col_count; ++col_index) {
            if (grid[row_index][col_index] == '@') {
                robot_row = row_index;
                robot_col = col_index;
                grid[row_index][col_index] = '.'; // Replace robot position with empty space
            }
        }
    }

    int current_row = robot_row, current_col = robot_col;

    for (char instruction : instructions) {
        pair<int, int> direction;
        if (instruction == '^') direction = direction_offsets[0];
        else if (instruction == '>') direction = direction_offsets[1];
        else if (instruction == 'v') direction = direction_offsets[2];
        else if (instruction == '<') direction = direction_offsets[3];
        else continue;

        int delta_row = direction.first, delta_col = direction.second;
        int next_row = current_row + delta_row, next_col = current_col + delta_col;

        if (grid[next_row][next_col] == '#') {
            continue; // Hit a wall, cannot move
        }
        else if (grid[next_row][next_col] == '.') {
            current_row = next_row;
            current_col = next_col;
        }
        else if (grid[next_row][next_col] == 'O' || grid[next_row][next_col] == '[' || grid[next_row][next_col] == ']') {
            deque<pair<int, int>> processing_queue;
            set<pair<int, int>> visited_positions;
            bool can_move = true;

            processing_queue.emplace_back(current_row, current_col);
            while (!processing_queue.empty()) {
                auto [queue_row, queue_col] = processing_queue.front();
                processing_queue.pop_front();
                if (visited_positions.count({ queue_row, queue_col })) continue;
                visited_positions.insert({ queue_row, queue_col });

                int next_queue_row = queue_row + delta_row, next_queue_col = queue_col + delta_col;
                if (grid[next_queue_row][next_queue_col] == '#') {
                    can_move = false;
                    break;
                }
                if (grid[next_queue_row][next_queue_col] == 'O') {
                    processing_queue.emplace_back(next_queue_row, next_queue_col);
                }
                if (grid[next_queue_row][next_queue_col] == '[') {
                    processing_queue.emplace_back(next_queue_row, next_queue_col);
                    assert(grid[next_queue_row][next_queue_col + 1] == ']');
                    processing_queue.emplace_back(next_queue_row, next_queue_col + 1);
                }
                if (grid[next_queue_row][next_queue_col] == ']') {
                    processing_queue.emplace_back(next_queue_row, next_queue_col);
                    assert(grid[next_queue_row][next_queue_col - 1] == '[');
                    processing_queue.emplace_back(next_queue_row, next_queue_col - 1);
                }
            }

            if (!can_move) continue;

            while (!visited_positions.empty()) {
                vector<pair<int, int>> sorted_positions(visited_positions.begin(), visited_positions.end());
                for (auto [pos_row, pos_col] : sorted_positions) {
                    int new_row = pos_row + delta_row, new_col = pos_col + delta_col;
                    if (visited_positions.count({ new_row, new_col }) == 0) {
                        assert(grid[new_row][new_col] == '.');
                        grid[new_row][new_col] = grid[pos_row][pos_col];
                        grid[pos_row][pos_col] = '.';
                        visited_positions.erase({ pos_row, pos_col });
                    }
                }
            }
            current_row += delta_row;
            current_col += delta_col;
        }
    }

    // Calculate the total GPS sum of all boxes
    int total_sum = 0;
    for (int row_index = 0; row_index < row_count; ++row_index) {
        for (int col_index = 0; col_index < col_count; ++col_index) {
            if (grid[row_index][col_index] == 'O' || grid[row_index][col_index] == '[') {
                total_sum += 100 * row_index + col_index;
            }
        }
    }

    return total_sum;
}

int main() {
    ifstream input_file("day15.txt");
    if (!input_file.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    string file_content((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    input_file.close();

    size_t delimiter_pos = file_content.find("\n\n");
    string grid_data = file_content.substr(0, delimiter_pos);
    string instruction_data = file_content.substr(delimiter_pos + 2);

    vector<string> grid;
    size_t line_start = 0, line_end;
    while ((line_end = grid_data.find('\n', line_start)) != string::npos) {
        grid.push_back(grid_data.substr(line_start, line_end - line_start));
        line_start = line_end + 1;
    }
    grid.push_back(grid_data.substr(line_start));

    cout << calculate_sum(grid, instruction_data, false) << endl;
    cout << calculate_sum(grid, instruction_data, true) << endl;

    return 0;
}
