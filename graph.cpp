// main.cpp or graph.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cctype>
using namespace std;         

// Trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

// CSV reader (fully std-free now)
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> result;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return result;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) continue;

        vector<string> row;
        string cell;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];

            if (c == '"') {
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    cell += '"';
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                row.push_back(trim(cell));
                cell.clear();
            } else {
                cell += c;
            }
        }
        row.push_back(trim(cell));
        if (!row.empty() && !(row.size() == 1 && row[0].empty()))
            result.push_back(row);
    }
    file.close();
    return result;
}

// Node & Edge structs
struct Node {
    string id;
    string label;
    string house;
};

struct Edge {
    string source;
    string target;
    int weight;
};

int main() {
    // Read nodes
    auto nodeData = readCSV("got-nodes.csv");
    unordered_map<string, Node> nodes;

    if (nodeData.empty()) {
        cout << "Failed to read got-nodes.csv\n";
        return 1;
    }

    // Skip header
    for (size_t i = 1; i < nodeData.size(); ++i) {
        const auto& row = nodeData[i];
        if (row.size() >= 3) {
            Node n;
            n.id = row[0];
            n.label = row[1];
            n.house = row.size() > 2 ? row[2] : "Unknown";
            nodes[n.id] = n;

            cout << "Node: " << n.label << " (House " << n.house << ")\n";
        }
    }

    // Read edges
    auto edgeData = readCSV("got-edges.csv");
    vector<Edge> edges;

    for (size_t i = 1; i < edgeData.size(); ++i) {
        const auto& row = edgeData[i];
        if (row.size() >= 2) {
            Edge e;
            e.source = row[0];
            e.target = row[1];
            e.weight = row.size() >= 3 && !row[2].empty() ? stoi(row[2]) : 1;
            edges.push_back(e);

            cout << nodes[e.source].label << " --(" << e.weight << ")--> "
                 << nodes[e.target].label << "\n";
        }
    }

    cout << "\nLoaded " << nodes.size() << " characters and "
         << edges.size() << " relationships!\n";

    // Build the actual graph (adjacency list)
    using Graph = unordered_map<string, vector<pair<string, int>>>;
    Graph graph;

    for (const auto& e : edges) {
        graph[e.source].emplace_back(e.target, e.weight);
        graph[e.target].emplace_back(e.source, e.weight);   // remove this line if directed
    }

    cout << "\nGraph built! Nodes with connections: " << graph.size() << "\n\n";

    // Example: show Jon Snow's connections (ID usually "1")
    if (nodes.count("1")) {
        cout << nodes["1"].label << "'s strongest allies:\n";
        for (const auto& [neighbor, weight] : graph["1"]) {
            cout << "  → " << nodes[neighbor].label << " (weight " << weight << ")\n";
        }
    }

    return 0;
}