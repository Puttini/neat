import matplotlib.pyplot as plt
import networkx as nx
import neat

def drawGraph( graph ):
    # Compute position of nodes
    layers = graph.getLayers()
    nbLayers = layers[ graph.nbInputs ] + 1
    layerSizes = [ 0 ] * nbLayers
    for l in layers:
        if l != -1:
            layerSizes[l] += 1

    currentPosPerLayer = [ float(s)/2. for s in layerSizes ]
    pos = {}

    # Construct Networkx Graph
    g = nx.Graph()

    for n, l in enumerate(layers):
        if l >= 0:
            g.add_node( n )
            pos[n] = ( l, currentPosPerLayer[l] )
            currentPosPerLayer[l] -= 1

    for connection in graph.connections:
        g.add_edge(
                connection.n0,
                connection.n1,
                weight = connection.w )

    nx.draw( g, with_labels=True, pos=pos )
    return plt

if __name__ == "__main__":
    my_graph = neat.Graph( 2, 1, init_connect=False )
    my_graph.connections = [
            neat.Connection(0,5),
            neat.Connection(1,5),
            neat.Connection(5,2),
            neat.Connection(0,3),
            neat.Connection(5,3),
            neat.Connection(3,2),
            neat.Connection(1,2) ]

    drawGraph( my_graph ).show()
