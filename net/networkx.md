```python
# coding:utf-8
import networkx as nx
import matplotlib.pyplot as plt
import random

# regular graph
# generate a regular graph which has 20 nodes & each node has 3 neghbour nodes.
G = nx.random_graphs.random_regular_graph(6, 40)

# print G.nodes()  # 输出全部的节点： [1, 2, 3]
# print G.edges()  # 输出全部的边：[(2, 3)]
# print G.number_of_edges()  # 输出边的数量：1
# print G.get_edge_data(0, 8)

print G.number_of_nodes()
for e in G.edges():
    w = random.randint(150, 850)
    print '%d %d %d' % (e[0], e[1], w)
    G[e[0]][e[1]]['weight'] = w
G = G.to_directed()

print nx.shortest_path(G, 0, 5, weight="weight")
print nx.shortest_path_length(G, 0, 5, weight="weight")

# the spectral layout
# pos = nx.spectral_layout(G)
# draw the regular graph
# nx.draw(G, pos, with_labels=False, node_size=30)
# plt.show()

```