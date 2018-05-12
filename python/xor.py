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
    ge.reset()
    score = 0 - ge.eval([0,0,1],5)
    fitness += score*score

    ge.reset()
    score = 1 - ge.eval([1,0,1],5)
    fitness += score*score

    ge.reset()
    score = 1 - ge.eval([0,1,1],5)
    fitness += score*score

    ge.reset()
    score = 0 - ge.eval([1,1,1],5)
    fitness += score*score

    return fitness

def train(
        population,
        nbGenerations ):

    ga = GenAlgo( 3, 1, population )
    ga.setSeed(0)
    ga.c12 = 2
    ga.c3 = 1
    ga.dThreshold = 10

    d = nbGenerations/5

    for gen in range(nbGenerations+1):
        fitnesses = [ computeFitness(g) for g in ga.genomes ]

        mean_fitness = np.array( fitnesses ).mean()
        print( "Generation %3d got a score of %2.6f with %3d species"
                % (gen,mean_fitness,len(ga.speciesRepresentants)) )

        if gen%d == 0:
            drawBest( ga, fitnesses, nbGenerations/d+1, 3, gen/d, "Generation %d" % gen )

        if gen < nbGenerations:
            ga.nextGen( fitnesses )
        else:
            best_ind = min( range(population), key = lambda i: fitnesses[i] )
            return ga, best_ind

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 3:
        pop = 150
        gen = 50
        print( "Taking default parameters: population=%d nbGenerations=%d" % (pop,gen) )
    else:
        pop = int(sys.argv[1])
        gen = int(sys.argv[2])

    ga, best = train( pop, gen )

    g = ga.genomes[best]
    ge = GraphEval(g)
    print( "Best table :" )
    ge.reset()
    print( " 0 | 0 => %f" % ge.eval([0,0,1],5) )
    ge.reset()
    print( " 0 | 1 => %f" % ge.eval([0,1,1],5) )
    ge.reset()
    print( " 1 | 0 => %f" % ge.eval([1,0,1],5) )
    ge.reset()
    print( " 1 | 1 => %f" % ge.eval([1,1,1],5) )

    plt.show()
