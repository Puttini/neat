import matplotlib.pyplot as plt
import networkx as nx
import neat

def nxGraph( graph ):
    g = nx.Graph()

    for node in graph.nodes:
        g.add_node( node.n )

    for connection in graph.connections:
        g.add_edge(
                connection.n0,
                connection.n1,
                weight = connection.w )

    return g

def drawGraph( graph ):
    g = nxGraph( graph )
    nx.draw( g, with_labels=True )
    plt.show()
