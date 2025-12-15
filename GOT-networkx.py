import networkx as nx
import csv 
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


G = nx.Graph()

# Load the dataset

# load nodes
nodes = csv.reader(open("datasets/got-nodes.csv"))
next(nodes)
for row in nodes: 
    G.add_nodes_from(row, color = 'blue')

# load edges
edges = csv.reader(open("datasets/got-edges.csv"))
next(edges)
for row in edges:
    if len(row) >= 3:
        u = row[0].strip()
        v = row[1].strip()
        w = int(row[2])

        if u and v:
            G.add_edge(u, v, weight = w)

# remove empty nodes
for n in G.nodes():
    if n == '':
        G.remove_node(n)

# color nodes according to their color attribute    
color_map = []
for n in G.nodes():
    color_map.append(G.nodes[n]['color'])

# Now let's try to implement a BFS between 2 nodes, for example between Jon (Snow) and Jaime (Lannister)
def bfs(G, start_id, end_id):

    # No weights need, our weight are trivial right now
    path = nx.shortest_path(G, start_id, end_id)
    length = nx.shortest_path_length(G, start_id, end_id)

    return path, length

print("Edges:", G.number_of_edges())
print(list(G.edges())[:10])

print("BFS from Jon Snow", bfs(G, "Jon", "Jaime"))



pos = nx.spring_layout(G, dim=3, seed=42)



fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

xs, ys, zs, colors = [], [], [], []

for n in G.nodes():
    x, y, z = pos[n]
    xs.append(x)
    ys.append(y)
    zs.append(z)
    colors.append(G.nodes[n]['color'])

ax.scatter(xs, ys, zs, c=colors, s=50)

for u, v in G.edges():
    ax.plot(
        [pos[u][0], pos[v][0]],
        [pos[u][1], pos[v][1]],
        [pos[u][2], pos[v][2]],
        color='gray',
        alpha=0.4
    )

ax.set_title("GOT Network — 3D")
ax.set_axis_off()

plt.show()
