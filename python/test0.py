import neat
import graph

g = neat.Graph()
g.nodes = [ neat.Node(i) for i in range(5) ]
g.connections = [
        neat.Connection(1,3, 1),
        neat.Connection(2,3, 1),
        neat.Connection(0,4, 1),
        neat.Connection(3,4, 1) ]

graph.drawGraph( g )
