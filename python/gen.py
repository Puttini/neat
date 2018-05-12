from neat import *
import numpy as np
import matplotlib.pyplot as plt
from graph import drawGraph

def onlyAddNode():
    pop = 3
    nbNodes = 4

    ga = GenAlgo( 2, 2, pop )
    ga.setSeed(0)
    ga.init()

    for p in range(pop):
        g = ga.genomes[p]

        plt.subplot(nbNodes+1,pop,p+1)
        drawGraph(g)

        for n in range(nbNodes):
            ga.addNode(g)

            plt.subplot(nbNodes+1,pop,p+(n+1)*pop+1)
            drawGraph(g, draw_dis=True)

def onlyAddConnection():
    pop = 3
    nbConnections = 4

    ga = GenAlgo( 2, 2, pop )
    ga.setSeed(0)
    ga.init()

    for g in ga.genomes:
        for _ in range(3):
            ga.addNode(g)

    for p in range(pop):
        g = ga.genomes[p]

        plt.subplot(nbConnections+1,pop,p+1)
        drawGraph(g)

        for n in range(nbConnections):
            ga.addConnection(g)

            plt.subplot(nbConnections+1,pop,p+(n+1)*pop+1)
            drawGraph(g, draw_dis=True)

def onlyChangeWeights():
    nbRows = 4
    nbCols = 3

    ga = GenAlgo(2,2,1)
    ga.setSeed(0)
    ga.init()

    g = ga.genomes[0]
    for _ in range(4):
        ga.addNode(g)

    for row in range(nbRows):
        for col in range(nbCols):
            if row != 0 or col != 0:
                for c in g.connections:
                    ga.changeWeight(c)
            plt.subplot(nbRows,nbCols,col+row*nbCols+1)
            drawGraph(g, use_pos=True, draw_dis=False)

    print( " --- Final connection weights ---" )
    for c in g.connections:
        if c.enabled:
            print( "%d -> %d : %f" % (c.n0, c.n1, c.w) )

def onlyCrossOver():
    nbGen = 3

    ga = GenAlgo(2,2,2)
    ga.setSeed(0)
    ga.init()

    g0, g1 = ga.genomes
    for _ in range(2):
        ga.addNode(g0)
        ga.addNode(g1)

    for _ in range(2):
        ga.addConnection(g0)
        ga.addConnection(g1)

    plt.subplot(nbGen+1,2,1)
    drawGraph(g0)
    plt.subplot(nbGen+1,2,2)
    drawGraph(g1)

    for gen in range(nbGen):
        new_gen = [
                ga.crossOver(
                    ga.genomes[0], ga.genomes[1],
                    1, 1 )
                for _ in range(2) ]
        ga.genomes = new_gen

        g0, g1 = ga.genomes

        plt.subplot(nbGen+1,2,2*(gen+1)+1)
        drawGraph(g0)
        plt.subplot(nbGen+1,2,2*(gen+1)+2)
        drawGraph(g1)

def basicGenerations():
    nbGen = 3

    ga = GenAlgo(2,1,5)
    ga.setSeed(0)
    ga.init()

    ga.pAddNode = 0.4
    ga.dThreshold = 2.
    ga.c3 = 2
    ga.init()

    for i, g in enumerate(ga.genomes):
        plt.subplot(nbGen+1,5,i+1)
        drawGraph(g)

    print( "Starting genetic algorithm" )
    print( "Generation %d: %d species" % (0, len(ga.popPerSpecies)) )
    for gen in range(nbGen):
        ga.nextGen( [ len(g.connections) for g in ga.genomes ] )

        for i, g in enumerate(ga.genomes):
            plt.subplot(nbGen+1,5,(gen+1)*5+i+1)
            drawGraph(g)

        print( "Generation %d: %d species" % (gen+1, len(ga.popPerSpecies)) )

if __name__ == "__main__":
    plt.figure("Only adding nodes")
    onlyAddNode()

    plt.figure("Only adding connections")
    onlyAddConnection()

    plt.figure("Only changing weights")
    onlyChangeWeights()

    plt.figure("Only cross over")
    onlyCrossOver()

    plt.figure( "Simple generations" )
    basicGenerations()

    plt.show()
