#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdexcept>

using namespace std;

// Registers
int A, B, C;

// Function to compute combo operand value
int combo_value(int operand) {
    if (operand <= 3) return operand;     // Literal values 0-3
    if (operand == 4) return A;           // Register A
    if (operand == 5) return B;           // Register B
    if (operand == 6) return C;           // Register C
    throw invalid_argument("Invalid combo operand");
}

// Program execution function
vector<int> run_program(const vector<int>& program, int initial_A) {
    vector<int> output;
    int ip = 0; // Instruction Pointer
    A = initial_A; B = 0; C = 0;

    while (ip < program.size()) {
        int opcode = program[ip];
        int operand = program[ip + 1];

        switch (opcode) {
        case 0: { // adv
            int denom = pow(2, combo_value(operand));
            A /= denom;
            break;
        }
        case 1: { // bxl
            B ^= operand;
            break;
        }
        case 2: { // bst
            B = combo_value(operand) % 8;
            break;
        }
        case 3: { // jnz
            if (A != 0) {
                ip = operand;
                continue;
            }
            break;
        }
        case 4: { // bxc
            B ^= C;
            break;
        }
        case 5: { // out
            output.push_back(combo_value(operand) % 8);
            break;
        }
        case 6: { // bdv
            int denom = pow(2, combo_value(operand));
            B = A / denom;
            break;
        }
        case 7: { // cdv
            int denom = pow(2, combo_value(operand));
            C = A / denom;
            break;
        }
        default:
            throw invalid_argument("Invalid opcode");
        }

        ip += 2; // Move to the next instruction
    }

    return output;
}

bool is_program_output_equal_to_input(const vector<int>& program, const vector<int>& output) {
    return program == output;
}

int find_min_initial_A(const vector<int>& program) {
    int initial_A = 1;
    while (true) {
        vector<int> output = run_program(program, initial_A);
        if (is_program_output_equal_to_input(program, output)) {
            return initial_A; // Found the lowest A
        }
        ++initial_A;
    }
}

// Function to read input from file
void read_input(const string& filename, vector<int>& program, int& initial_A) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    string line;
    while (getline(file, line)) {
        if (line.find("Register A:") != string::npos) {
            initial_A = stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Register B:") != string::npos) {
            B = stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Register C:") != string::npos) {
            C = stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Program:") != string::npos) {
            stringstream ss(line.substr(line.find(":") + 1));
            int value;
            while (ss >> value) {
                program.push_back(value);
                if (ss.peek() == ',') ss.ignore();
            }
        }
    }
    file.close();
}

int main() {
    string filename = "day17";
    vector<int> program;
    int initial_A;

    try {
        // Read input from file
        read_input(filename, program, initial_A);

        // Part 1: Run program with the given value of A
        cout << "Part 1: Output of the program with given A = " << initial_A << ":\n";
        vector<int> part1_output = run_program(program, initial_A);
        for (size_t i = 0; i < part1_output.size(); ++i) {
            cout << part1_output[i];
            if (i != part1_output.size() - 1) cout << ",";
        }
        cout << endl;

        // Part 2: Find the lowest positive A that outputs a copy of the program
        cout << "Part 2: Finding lowest positive A that outputs a copy of the program...\n";
        int result = find_min_initial_A(program);
        cout << "Lowest positive initial value for A: " << result << endl;

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
