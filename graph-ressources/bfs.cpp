#include <list>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <cstdio>
#include <algorithm>
#include <bits/stdc++.h>
#include <utility>
#include <math.h>

#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>

using namespace std;

// Queremos encontrar el minimo camino de n a m, esto grita a 4 vientos BFS
// Rdo: BFS me devuelve la distancia minima entre 2 nodos
// En nuestro caso las distancias van a ser todas = 1, pues es la cantidad de veces que aprieto el boton
// Los nodos serán el valor mostrado en la pantalla en ese momento dado
long long bfs(long long n, long long m) {
    if (n == m) return 0;

    const long long MAX = 1000000;
    vector<bool> visitado(MAX + 1, false);              // Nodos no visitados
    queue<pair<long long, long long>> nodo;             // {nodo_actual, pasos}

    nodo.push(make_pair(n, 0));                         // Inicializamos en n con 0 pasos
    visitado[n] = true;                   

    while (!nodo.empty()) {
        long long actual = nodo.front().first;  
        long long paso = nodo.front().second;  
        nodo.pop();

        // Rojo -> n = n * 2
        long long rojo = actual * 2;
        if (rojo <= MAX && !visitado[rojo]) {
            if (rojo == m) return paso + 1;
            visitado[rojo] = true;
            nodo.push(make_pair(rojo, paso + 1));
        }

        // Azul -> n = n - 1
        long long azul = actual - 1;
        if (azul >= 1 && !visitado[azul]) {
            if (azul == m) return paso + 1;
            visitado[azul] = true;
            nodo.push(make_pair(azul, paso + 1));
        }
    }

    return -1; // aunque siempre vamos a encontrar una solucion.
}


int main() {
    int n, m;
    cin >> n >> m;

    cout << bfs(n, m) << endl;
    return 0;
}
