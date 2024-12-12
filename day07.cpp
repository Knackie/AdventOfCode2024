#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// Fonction pour évaluer une expression avec les opérateurs '+', '*', et '||'
long long evaluateExpression(const vector<long long>& numbers, const vector<string>& operators) {
    long long result = numbers[0];
    string currentConcat = to_string(numbers[0]);
    
    for (size_t i = 0; i < operators.size(); ++i) {
        if (operators[i] == "+") {
            result += numbers[i + 1];
            currentConcat = to_string(result);
        } else if (operators[i] == "*") {
            result *= numbers[i + 1];
            currentConcat = to_string(result);
        } else if (operators[i] == "||") {
            currentConcat += to_string(numbers[i + 1]);
            result = stoll(currentConcat);
        }
    }
    return result;
}

// Fonction récursive pour tester toutes les combinaisons possibles d'opérateurs
bool canMatchTarget(long long target, const vector<long long>& numbers, vector<string>& operators, size_t index) {
    if (index == numbers.size() - 1) {
        return evaluateExpression(numbers, operators) == target;
    }

    // Tester tous les opérateurs : '+', '*', '||'
    for (const string& op : {"+", "*", "||"}) {
        operators[index] = op;
        if (canMatchTarget(target, numbers, operators, index + 1)) {
            return true;
        }
    }
    return false;
}

int main() {
    ifstream inputFile("day7.txt");
    if (!inputFile) {
        cerr << "Erreur: Impossible d'ouvrir le fichier 'day7.txt'." << endl;
        return 1;
    }

    string line;
    long long totalCalibrationResult = 0;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        string targetString;
        iss >> targetString;
        if (targetString.empty()) continue;

        long long target = stoll(targetString);

        vector<long long> numbers;
        long long num;
        while (iss >> num) {
            numbers.push_back(num);
        }

        if (numbers.empty()) continue;

        // Initialiser les opérateurs
        vector<string> operators(numbers.size() - 1);

        // Vérifier si une combinaison rend la cible valide
        if (canMatchTarget(target, numbers, operators, 0)) {
            totalCalibrationResult += target;
        }
    }

    inputFile.close();

    cout << "Total calibration result with concatenation: " << totalCalibrationResult << endl;

    return 0;
}
