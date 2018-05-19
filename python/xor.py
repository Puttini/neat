from neat import *
from graph import *
import numpy as np

nb_eval = 3

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
    score = 0 - ge.eval([0,0,1],nb_eval)
    fitness += score*score

    ge.reset()
    score = 1 - ge.eval([1,0,1],nb_eval)
    fitness += score*score

    ge.reset()
    score = 1 - ge.eval([0,1,1],nb_eval)
    fitness += score*score

    ge.reset()
    score = 0 - ge.eval([1,1,1],nb_eval)
    fitness += score*score

    fitness += g.getNbNodes()/10

    return pow( fitness, 1 )

def train(
        population,
        nbGenerations ):

    ga = GenAlgo( 3, 1, population )
    ga.setSeed(0)

    ga.c12 = 1
    ga.c3 = 2
    ga.dThreshold = 0.5

    ga.initStdDev = 1.0
    ga.defStdDev = 0.5
    ga.relStdDev = 0.2

    ga.init()

    d = nbGenerations/5

    for gen in range(nbGenerations+1):
        fitnesses = [ computeFitness(g) for g in ga.genomes ]

        mean_fitness = np.array( fitnesses ).mean()
        print( "Generation %3d got a score of %9.6f with %3d species"
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

    plt.figure( "Training result" )
    ga, best = train( pop, gen )

    g = ga.genomes[best]
    ge = GraphEval(g)
    print( "Best table :" )
    ge.reset()
    print( " 0 | 0 => %f" % ge.eval([0,0,1],nb_eval) )
    ge.reset()
    print( " 0 | 1 => %f" % ge.eval([0,1,1],nb_eval) )
    ge.reset()
    print( " 1 | 0 => %f" % ge.eval([1,0,1],nb_eval) )
    ge.reset()
    print( " 1 | 1 => %f" % ge.eval([1,1,1],nb_eval) )

    plt.figure( "Final network" )

    plt.subplot( 2, 1, 1 )
    drawGraph( g )
    plt.subplot( 2, 1, 2 )
    drawGraph( g.simplify() )

    plt.show()
