#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <tuple>
#include <cmath>

struct Machine {
    int a_x, a_y, b_x, b_y;
    long long prize_x, prize_y;
};

// Function to calculate the minimum cost using a simple linear Diophantine equation solver
long long calculate_cost(const Machine& machine, int max_presses) {
    long long min_cost = std::numeric_limits<long long>::max();

    for (int a_count = 0; a_count <= max_presses; ++a_count) {
        for (int b_count = 0; b_count <= max_presses; ++b_count) {
            long long x = a_count * machine.a_x + b_count * machine.b_x;
            long long y = a_count * machine.a_y + b_count * machine.b_y;

            if (x == machine.prize_x && y == machine.prize_y) {
                long long cost = a_count * 3 + b_count;
                min_cost = std::min(min_cost, cost);
            }
        }
    }

    return min_cost == std::numeric_limits<long long>::max() ? -1 : min_cost;
}

int main() {
    std::ifstream file("day13.txt");
    std::string line;
    std::vector<Machine> machines;

    // Parse the input file
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Machine machine;
        sscanf(line.c_str(), "Button A: X+%d, Y+%d", &machine.a_x, &machine.a_y);
        std::getline(file, line);
        sscanf(line.c_str(), "Button B: X+%d, Y+%d", &machine.b_x, &machine.b_y);
        std::getline(file, line);
        sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &machine.prize_x, &machine.prize_y);

        // Adjust prize coordinates as per Part Two
        machine.prize_x += 10000000000000LL;
        machine.prize_y += 10000000000000LL;

        machines.push_back(machine);
    }

    // Calculate the minimum tokens required to win prizes
    long long total_cost = 0;
    int prizes_won = 0;
    int max_presses = 10000; // Extend the range significantly

    for (const auto& machine : machines) {
        long long cost = calculate_cost(machine, max_presses);
        if (cost != -1) {
            total_cost += cost;
            ++prizes_won;
        }
        else {
            std::cerr << "No solution found for prize at (" << machine.prize_x << ", " << machine.prize_y << ")" << std::endl;
        }
    }

    std::cout << "Prizes won: " << prizes_won << std::endl;
    std::cout << "Total cost: " << total_cost << std::endl;

    return 0;
}
