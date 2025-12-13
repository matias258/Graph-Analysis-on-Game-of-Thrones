// main.cpp or graph.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <queue>
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

// We want to obtain the distance from John Snow to Jaime Lannister
// Lets define a BFS Search
using Graph = unordered_map<string, vector<pair<string, int>>>;
int bfs(const Graph& G, const string& startId, const string& endId){
    if (startId == endId){
        return 0;
    }

    unordered_map<string, int> distance;
    queue<string> q;

    while (!q.empty()){
        string currentId = q.front();
        q.pop();

        int currentDist = distance.at(currentId);

        // Si encontramos el nodo destino
        if (currentId == endId){
            return currentDist;
        }

        // Iterar sobre los vecinos (pares<neighborId, weight>)
        if (G.count(currentId)){
            for (const auto& edge: G.at(currentId)){
                const string& neighborId = edge.first;

                // Si el vecino no ha sido visitado (no esta en el mapa distance)
                if (distance.find(neighborId) == distance.end()){
                    distance[neighborId] = currentDist + 1;
                    q.push(neighborId);
                }
            }
        }
    }

    // Destino inalcanzable
    return -1;

}

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

    // ... (El código anterior de tu main)

    cout << "\nGraph built! Nodes with connections: " << graph.size() << "\n\n";

    // =================================================================
    // CÁLCULO DE LA DISTANCIA (BFS) - CÓDIGO CORREGIDO
    // =================================================================

    string jonSnowID = "Jon";       // ID correcto para Jon Snow
    string jaimeLannisterID = "Jaime";  // ID CORRECTO para Jaime Lannister (antes era "Val")

    // Verificación de existencia de los nodos
    if (nodes.count(jonSnowID) && nodes.count(jaimeLannisterID)) {
        
        // Llama a la función BFS
        int shortestDistance = bfs(graph, jonSnowID, jaimeLannisterID); 

        cout << "\n--- Análisis de Distancia (BFS) ---\n";
        cout << "Buscando la distancia más corta entre " << nodes.at(jonSnowID).label 
             << " y " << nodes.at(jaimeLannisterID).label << "...\n";

        if (shortestDistance != -1) {
            cout << "Distancia encontrada: **" << shortestDistance << " relaciones**.\n";
            // Puedes agregar aquí la lógica para imprimir el camino, si la implementaste en BFS.
        } else {
            cout << "No existe un camino directo entre " << nodes.at(jonSnowID).label 
                 << " y " << nodes.at(jaimeLannisterID).label << " (Inalcanzable).\n";
        }
    } else {
        // Mensaje de error ajustado para reflejar los IDs buscados
        cout << "\nError de Análisis: Los personajes 'Jon' o 'Jaime' no se encontraron en los datos cargados.\n";
    }

    // =================================================================

    return 0;
}