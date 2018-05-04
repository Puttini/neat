from neat import *
import numpy as np
import matplotlib.pyplot as plt
from graph import drawGraph

def onlyAddNode():
    pop = 3
    nbNodes = 4

    ga = GenAlgo( 2, 2, pop )
    ga.setSeed(0)

    for p in range(pop):
        g = ga.genomes[p]

        plt.subplot(nbNodes+1,pop,p+1)
        drawGraph(g)

        for n in range(nbNodes):
            ga.addNode(g)

            plt.subplot(nbNodes+1,pop,p+(n+1)*pop+1)
            drawGraph(g)

    plt.show()

if __name__ == "__main__":
    onlyAddNode()
