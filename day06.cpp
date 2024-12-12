#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cstring>

int main(int argc, char *argv[]) {
    std::string inputFile = argc >= 2 ? argv[1] : "6.txt";
    std::ifstream file(inputFile);
    std::string data;
    std::getline(file, data);

    int R, C;
    std::vector<std::string> grid;
    std::istringstream iss(data);
    for (std::string line; std::getline(iss, line, '\n'); grid.push_back(line)) {
        R++; C = int(line.size());
    }

    int p1 = 0;
    int p2 = 0;

    int sr = -1, sc = -1; // Position du début
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == '^') {
                sr = r; sc = c;
            }
        }
    }

    std::unordered_set<std::pair<int, int>, utils::PairHash> seen_rc;
    for (int o_r = 0; o_r < R; ++o_r) {
        for (int o_c = 0; o_c < C; ++o_c) {
            int r = sr, c = sc;
            int d = 0; // 0=up, 1=right, 2=down, 3=left
            bool escape = false;
            int num_seen_rc = 0;
            std::unordered_set<std::tuple<int, int, int>, utils::TupleHash> seen;
            while (true) {
                if (seen.count({r, c, d})) {
                    p2++;
                    break;
                }
                seen.insert({r, c, d});
                std::tuple<int, int, bool> rc_entry(r, c, escape);
                if (seen_rc.count(rc_entry)) {
                    p1 = num_seen_rc;
                    break;
                }
                seen_rc.insert(std::move(rc_entry));
                num_seen_rc++;

                int dir = [(-1, 0, 0), (0, 1, 0), (1, 0, 0), (0, -1, 0)][d];
                int rr = r + dir[0];
                int cc = c + dir[1];
                escape = dir[2];

                if (rr < 0 || rr >= R || cc < 0 || cc >= C) {
                    if (grid[o_r][o_c] == '#') {
                        p1 = num_seen_rc;
                    }
                    break;
                }

                if (grid[rr][cc] == '#' || (rr == o_r && cc == o_c)) {
                    d = (d + 1) % 4;
                } else {
                    r = rr;
                    c = cc;
                }
            }
        }
    }

    std::cout << "Partie 1 : " << p1 << std::endl;
    std::cout << "Partie 2 : " << p2 << std::endl;

    return 0;
}
