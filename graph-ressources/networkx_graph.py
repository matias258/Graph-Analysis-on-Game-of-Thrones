import networkx as nx
# link: https://networkx.org/documentation/stable/tutorial.html#creating-a-graph

# NODOS

G = nx.Graph()
G.add_node(1)
G.add_nodes_from([2, 3])

#G.add_nodes_from([(4, {"color: red"}), (5, {"color: green"})])




# Aristas (Edges)

G.add_edge(1, 2)
e = (2, 3)
G.add_edge(*e)
G.add_edges_from([(1,2), (1,3)])



print("Number of edges: ",G.number_of_edges())
print("Number of nodes: ", G.number_of_nodes(), "\n")

G.clear()


# Ejemplo 1 de Grafo
G.add_edges_from([(1, 2), (1, 3)])
G.add_node(1)
G.add_edge(1, 2)
G.add_node("spam")        # adds node "spam"
G.add_nodes_from("spam")  # adds 4 nodes: 's', 'p', 'a', 'm'
G.add_edge(3, 'm')

print("Number of edges: ",G.number_of_edges())
print("Number of nodes: ", G.number_of_nodes())
print("List of nodes: ", list(G.nodes()))
print("List of edges: ", list(G.edges()), "\n")



# Ejemplo 2
DG = nx.DiGraph()
DG.add_edge(2, 1)   # adds the nodes in order 2, 1
DG.add_edge(1, 3)
DG.add_edge(2, 4)
DG.add_edge(1, 2)
assert list(DG.successors(2)) == [1, 4]
assert list(DG.edges) == [(2, 1), (2, 4), (1, 3), (1, 2)]

print("Number of edges: ",DG.number_of_edges())
print("Number of nodes: ", DG.number_of_nodes())
print("List of nodes: ", list(DG.nodes()))
print("List of edges: ", list(DG.edges()))
print("List of neighbors of node 1: ", list(G.neighbors(1)))
print("Degree of node 1: ", G.degree[1], "\n")




# Access patterns

G.nodes["spam"]["color"] = "blue"
G.edges[(1, 2)]["weight"] = 10

print("Number of edges: ",G.number_of_edges())
print("Number of nodes: ", G.number_of_nodes())
print("List of nodes: ", list(G.nodes()))
print("List of edges: ", list(G.edges()), "\n")


# Removing elements from a graph
