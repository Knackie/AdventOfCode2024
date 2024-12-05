#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <set>
using namespace std;

// Lire les règles et les mises à jour depuis un fichier
void parseInput(const string& filename, vector<pair<int, int>>& rules, vector<vector<int>>& updates) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl; exit(1); }

    string line;
    bool parsingUpdates = false; // Permet de différencier les règles des mises à jour
    while (getline(file, line)) {
        if (line.empty()) {
            parsingUpdates = true; // Une ligne vide sépare les règles des mises à jour
        }
        else if (parsingUpdates) {
            // Parser une mise à jour
            vector<int> update;
            size_t pos = 0;
            while ((pos = line.find(',')) != string::npos) {
                update.push_back(stoi(line.substr(0, pos)));
                line.erase(0, pos + 1);
            }
            update.push_back(stoi(line)); // Dernière valeur
            updates.push_back(update);
        }
        else {
            // Parser une règle
            size_t delim = line.find('|');
            int x = stoi(line.substr(0, delim));
            int y = stoi(line.substr(delim + 1));
            rules.emplace_back(x, y);
        }
    }
}

// Vérifie si une mise à jour respecte les règles
bool isOrdered(const vector<int>& update, const vector<pair<int, int>>& rules) {
    unordered_map<int, int> positions;
    for (int i = 0; i < update.size(); ++i) positions[update[i]] = i;

    for (auto& [x, y] : rules) {
        if (positions.count(x) && positions.count(y) && positions[x] > positions[y]) {
            return false; // Règle violée
        }
    }
    return true;
}

// Réordonne une mise à jour en utilisant les règles
vector<int> reorder(const vector<int>& update, const vector<pair<int, int>>& rules) {
    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> inDegree;
    set<int> pages(update.begin(), update.end());

    // Construire le graphe d'adjacence
    for (auto& [x, y] : rules) {
        if (pages.count(x) && pages.count(y)) {
            graph[x].push_back(y);
            inDegree[y]++;
        }
    }

    // Tri topologique pour ordonner les pages
    queue<int> q;
    vector<int> sorted;
    for (int page : update) if (!inDegree[page]) q.push(page);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        sorted.push_back(current);
        for (int neighbor : graph[current]) {
            if (--inDegree[neighbor] == 0) q.push(neighbor);
        }
    }
    return sorted;
}

// Fonction principale
int main() {
    vector<pair<int, int>> rules;
    vector<vector<int>> updates;

    parseInput("day5", rules, updates);

    int sumOrdered = 0, sumCorrected = 0;

    for (const auto& update : updates) {
        if (isOrdered(update, rules)) {
            sumOrdered += update[update.size() / 2]; // Page du milieu pour les mises à jour ordonnées
        }
        else {
            auto corrected = reorder(update, rules);
            sumCorrected += corrected[corrected.size() / 2]; // Page du milieu pour les mises à jour corrigées
        }
    }

    cout << "Partie 1 : " << sumOrdered << endl;
    cout << "Partie 2 : " << sumCorrected << endl;

    return 0;
}
