#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Function to check if a design can be formed using the given towel patterns
bool canMatch(const string& design, const vector<string>& patterns, unordered_map<string, bool>& memo) {
    if (memo.find(design) != memo.end()) {
        return memo[design];
    }
    if (design.empty()) {
        return true; // Fully matched the design
    }

    for (const string& pattern : patterns) {
        if (design.find(pattern) == 0) { // Check if the design starts with the pattern
            if (canMatch(design.substr(pattern.size()), patterns, memo)) {
                memo[design] = true;
                return true;
            }
        }
    }

    memo[design] = false;
    return false;
}

// Function to count the number of ways a design can be formed using the given towel patterns
long long countWays(const string& design, const vector<string>& patterns, unordered_map<string, long long>& memo) {
    if (memo.find(design) != memo.end()) {
        return memo[design];
    }
    if (design.empty()) {
        return 1; // One way to match an empty design
    }

    long long ways = 0;
    for (const string& pattern : patterns) {
        if (design.find(pattern) == 0) { // Check if the design starts with the pattern
            ways += countWays(design.substr(pattern.size()), patterns, memo);
        }
    }

    memo[design] = ways;
    return ways;
}

int main() {
    ifstream inputFile("day19.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file day19.txt" << endl;
        return 1;
    }

    vector<string> towelPatterns;
    vector<string> designs;
    string line;
    bool isPatterns = true;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            isPatterns = false;
            continue;
        }
        if (isPatterns) {
            size_t pos = 0;
            while ((pos = line.find(", ")) != string::npos) {
                towelPatterns.push_back(line.substr(0, pos));
                line.erase(0, pos + 2);
            }
            if (!line.empty()) {
                towelPatterns.push_back(line);
            }
        }
        else {
            designs.push_back(line);
        }
    }

    inputFile.close();

    // Part 1: Count possible designs
    int possibleDesigns = 0;
    for (const string& design : designs) {
        unordered_map<string, bool> memo;
        if (canMatch(design, towelPatterns, memo)) {
            ++possibleDesigns;
        }
    }

    // Part 2: Total arrangements
    long long totalWays = 0;
    for (const string& design : designs) {
        unordered_map<string, long long> memo;
        totalWays += countWays(design, towelPatterns, memo);
    }

    cout << "Number of possible designs: " << possibleDesigns << endl;
    cout << "Total number of arrangements: " << totalWays << endl;

    return 0;
}
