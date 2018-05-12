from neat import *
from graph import *
import numpy as np

def drawBest( ga, fitnesses, nbRows, nbBest, row, title = None ):
    best_inds = sorted( range(len(fitnesses)), key=lambda i: fitnesses[i] )[:nbBest]

    for i, idx in enumerate(best_inds):
        plt.subplot(nbRows,nbBest,row*nbBest+i+1)
        drawGraph( ga.genomes[idx] )
        if i == (nbBest-1)/2 and title is not None:
            plt.title(title)

def computeFitness( g ):
    ge = GraphEval(g)

    fitness = 0

    # Construct a verity table, with a max number of iter of 5
    score = 0 - ge.eval([0,0,1],5)
    fitness += score*score

    score = 1 - ge.eval([1,0,1],5)
    fitness += score*score

    score = 1 - ge.eval([0,1,1],5)
    fitness += score*score

    score = 0 - ge.eval([1,1,1],5)
    fitness += score*score

    return fitness

def train(
        population,
        nbGenerations ):

    ga = GenAlgo( 3, 1, population )
    ga.setSeed(0)
    ga.dThreshold = 100

    d = 2

    for gen in range(nbGenerations+1):
        fitnesses = [ computeFitness(g) for g in ga.genomes ]

        if gen%d == 0:
            drawBest( ga, fitnesses, nbGenerations/d+1, 5, gen/d, "Generation %d" % gen )

        if gen < nbGenerations:
            ga.nextGen( fitnesses )
        else:
            best_ind = max( range(population), key = lambda i: fitnesses[i] )
            return ga, best_ind

if __name__ == "__main__":
    ga, best = train( 150, 10 )

    g = ga.genomes[best]
    ge = GraphEval(g)
    print( "Best table :" )
    print( " 0 | 0 => %f" % ge.eval([0,0,1],5) )
    print( " 0 | 1 => %f" % ge.eval([0,1,1],5) )
    print( " 1 | 0 => %f" % ge.eval([1,0,1],5) )
    print( " 1 | 1 => %f" % ge.eval([1,1,1],5) )

    plt.show()
