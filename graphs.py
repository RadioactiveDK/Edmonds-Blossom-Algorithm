import matplotlib.pyplot as plt
import networkx as nx
import random

# Read data from the input file
with open('graph_input.txt', 'r') as file:
    num_vertices = int(file.readline().strip())  # Read the number of vertices
    vertices = file.readline().strip().split()  # Read the vertex names as a list
    adjacency_matrix = [list(map(int, line.split())) for line in file]  # Read the adjacency matrix

# Create a random layout for the nodes
node_positions = {vertex: (random.uniform(0, 1), random.uniform(0, 1)) for vertex in vertices}

# Create a graph
G = nx.Graph()

# Add nodes and edges
for vertex in vertices:
    G.add_node(vertex)

for i in range(num_vertices):
    for j in range(i + 1, num_vertices):
        if adjacency_matrix[i][j] == 1:
            G.add_edge(vertices[i], vertices[j])

# Create a Matplotlib figure and axis
fig, ax = plt.subplots(figsize=(10, 7))

# Draw nodes
nx.draw_networkx_nodes(G, node_positions, node_color='lightblue', node_size=300)

# Draw edges with colors and thickness
edge_colors = ['gray' if adjacency_matrix[i][j] == 1 else 'red' for i in range(num_vertices) for j in range(i + 1, num_vertices)]
edge_widths = [1 if color == 'gray' else 2 for color in edge_colors]
nx.draw_networkx_edges(G, node_positions, width=edge_widths, edge_color=edge_colors)

# Draw node labels
nx.draw_networkx_labels(G, node_positions, font_size=10, font_color='black', font_weight='bold')

# Set the plot title
plt.title('Graph Visualization')

# Display the graph
plt.axis('off')  # Hide axis
plt.show()

