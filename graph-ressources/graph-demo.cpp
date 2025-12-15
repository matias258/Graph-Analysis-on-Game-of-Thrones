#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class Graph
{
    public:
        Graph() {}
        void addVertex(int vertex)
        {
            m_graph[vertex] = {};
        }

        void addEdge(int v1, int v2)
        {
            m_graph[v1].push_back(v2);
            m_graph[v2].push_back(v1); // grafo bidireccional
        }

        void printGraph()
        {
            for (const auto& element : m_graph)
            {
                cout << element.first << " : ";
                for (const auto& curEdge : element.second)
                {
                    cout << curEdge << " ";
                }
                cout << "\n";
            }
        }

        void removeVertex(int vertex){
            if (m_graph.find(vertex) == m_graph.end()){
                cout << "Invalid vertex\n";
                return;
            }

            m_graph.erase(vertex);

            for (auto& element : m_graph){
                auto& value = element.second;
                value.erase(remove(value.begin(), value.end(), vertex), value.end()); 
            }
        }

        vector<int> getVertices(){
            cout << "vertices: ";
            vector<int> vertices;

            for (const auto& element : m_graph){
                vertices.push_back(element.first);
                cout << element.first << " ";
            }
            cout << "\n";
            return vertices;
        }

        vector<int> getEdges(int vertex){
            if (m_graph.find(vertex) == m_graph.end()){
                cout << "Invalid vertex\n";
                return {};
            }

            cout << "edges: ";
            for (const auto& i : m_graph[vertex]){
                cout << i << " ";
            }
            cout << "\n";
            return m_graph[vertex];
        }

private:
        map<int, vector<int>> m_graph;
};

int main(){
    Graph myGraph;
    myGraph.addVertex(1);
    myGraph.addVertex(2);
    myGraph.addVertex(3);
    myGraph.addEdge(1,2);
    myGraph.addEdge(2,3);
    myGraph.addEdge(1,3);
    //myGraph.removeVertex(3);
    myGraph.getVertices();
    myGraph.getEdges(1);
    myGraph.printGraph();
    
    return 0;
}