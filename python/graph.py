import matplotlib.pyplot as plt
import networkx as nx
import neat

def drawGraph( graph, use_pos=True, draw_dis=False ):
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
    g = nx.MultiDiGraph()

    for n, l in enumerate(layers):
        if l >= 0:
            g.add_node( n )
            pos[n] = ( l, currentPosPerLayer[l] )
            currentPosPerLayer[l] -= 1

    for connection in graph.connections:
        if not connection.enabled and not draw_dis:
            continue
        elif not connection.enabled:
            c = (0.7,0.7,0.7)
            w = 1
        elif connection.w < 0:
            c = (1.,0.,0.)
            w = -connection.w
        elif connection.w > 0:
            c = (0.,1.,0.)
            w = 1
        else:
            c = (0.,0.,1.)
            w = connection.w
        w = min( max( w, 0.6 ), 4 )

        g.add_edge(
                connection.n0,
                connection.n1,
                color=c,
                weight=w )

    edges = g.edges()
    colors = [ g[u][v][0]['color'] for u,v in edges ]
    widths = [ g[u][v][0]['weight'] for u,v in edges ]

    try:
        if use_pos:
            nx.draw( g, with_labels=True, pos=pos, edge_color=colors, width=widths )
        else:
            nx.draw( g, with_labels=True, edge_color=colors, width=widths )
    except Exception:
        import ipdb; ipdb.set_trace()
    return plt

if __name__ == "__main__":
    my_graph = neat.Graph( 2, 1, init_connect=False )
    my_graph.connections = [
            neat.Connection(0,5),
            neat.Connection(1,5),
            neat.Connection(5,2),
            neat.Connection(0,3),
            neat.Connection(5,3),
            neat.Connection(3,5),
            neat.Connection(3,2),
            neat.Connection(1,2) ]

    plt.subplot(2,2,3)
    drawGraph( my_graph )

    plt.subplot(2,2,1)
    drawGraph( my_graph, use_pos=False )

# ---------------------------------------------------------------------------

    ga = neat.GenAlgo( 2, 1, 1 )
    ga.setSeed(0)
    g0 = ga.genomes[0]

    plt.subplot(2,2,2)
    drawGraph( g0, draw_dis=True )

    ga.addNode( g0 )

    plt.subplot(2,2,4)
    drawGraph( g0, draw_dis=True )

    plt.show()
