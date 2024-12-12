#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

// Function to solve the disk map problem
long long solve(const string& diskMap, bool part2) {
    deque<tuple<int, int, int>> files; // Store files as (position, size, id)
    deque<pair<int, int>> freeSpaces;  // Store free spaces as (position, size)
    vector<int> layout;                // Final disk layout
    int fileId = 0, pos = 0;

    // Parse the disk map
    for (size_t i = 0; i < diskMap.size(); ++i) {
        int size = diskMap[i] - '0';
        if (i % 2 == 0) { // File segment
            if (part2) files.emplace_back(pos, size, fileId);
            for (int j = 0; j < size; ++j) {
                layout.push_back(fileId);
                if (!part2) files.emplace_back(pos, 1, fileId);
                ++pos;
            }
            ++fileId;
        } else { // Free space segment
            freeSpaces.emplace_back(pos, size);
            layout.insert(layout.end(), size, -1); // Free spaces as -1
            pos += size;
        }
    }

    // Compact files into free spaces
    for (auto it = files.rbegin(); it != files.rend(); ++it) {
        auto [filePos, fileSize, id] = *it;
        for (auto& [spacePos, spaceSize] : freeSpaces) {
            if (spacePos < filePos && fileSize <= spaceSize) {
                for (int i = 0; i < fileSize; ++i) {
                    assert(layout[filePos + i] == id);
                    layout[filePos + i] = -1;
                    layout[spacePos + i] = id;
                }
                spacePos += fileSize;
                spaceSize -= fileSize;
                break;
            }
        }
    }

    // Calculate checksum
    long long checksum = 0;
    for (size_t i = 0; i < layout.size(); ++i) {
        if (layout[i] != -1) checksum += i * layout[i];
    }

    return checksum;
}

int main() {
    // Read input file
    ifstream file("9.txt");
    if (!file) {
        cerr << "Error: Cannot open file 9.txt" << endl;
        return 1;
    }

    string diskMap;
    file >> diskMap;

    // Solve for both parts
    long long part1 = solve(diskMap, false);
    long long part2 = solve(diskMap, true);

    // Print results
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
