#ifndef GEN_ALGO_HPP
#define GEN_ALGO_HPP

#include "Graph.hpp"
#include <random>
#include <map>

struct GenAlgo
{
    int nbInputs;
    int nbOutputs;
    int population;

    // Mutation probabilities
    float pAddNode;
    float pAddConnection;
    float pChangeWeight;
    float pDisableConnection;
    float pEnableConnection;
    float pTakeNewGene; // Experimental. Should always be 1.

    // During the computation of the next generation,
    // we keep nbSurvivors genomes, and compute (population-nbSurvivors)
    // offsprings from best selected genomes
    // We mutate all new genomes (survivors and offsprings)
    int nbSurvivors;
    int nbChildren;
    float interSpeciesRate;

    int nbMaxTry;

    // For choosing/changing weights
    float initStdDev; // Initialization around zero
    float defStdDev;  // During mutation :
    float relStdDev;  // w = normal( w, defStdDev + w*relStdDev )

    // For computing compatibility distance
    float c12, c3;    // We don't make distinction
    float dThreshold; // between excess and disjoint genes

    int current_inno;
    int current_node; // Next node to be added to the graph
    int current_generation;

    std::vector<Graph> genomes;

    std::vector<int> speciesPerGenome;     // Index of species per genome
    std::vector<int> speciesRepresentants; // Index of genome per species
    std::vector<int> popPerSpecies;        // Nb of genomes per species

    std::default_random_engine rng;

    bool is_init;

// ---------------------------------------------------------------------------

    GenAlgo() = default;
    GenAlgo( int nbInputs, int nbOutputs, int population = 150 );

    void setSeed( int seed );
    void init(); // Call this after setting parameters, and before calling any other function

    bool mutate_all( std::vector<Graph>& someGenomes );
    bool mutate( Graph& g );
    bool addNode( Graph& g );
    bool addConnection( Graph& g );
    bool changeWeight( Connection& c );
    bool initWeight( Connection& c );

    Graph crossOver(
            const Graph& g0, const Graph& g1,
            float fitness0, float fitness1 );

    int getNbMaxGenes() const;
    float computeCompDist(
            const Graph& g0, const Graph& g1,
            int nbMaxGenes ) const;

    void initSpecies();

    // Returns the map of old species VS new species, so you can trace them
    std::map<int,int> actualizeSpecies( const std::vector<Graph>& newGenomes );

    // Constructs the next generation, according to the vector of
    // fitnesses per genome
    std::map<int,int> nextGen( const std::vector<float>& fitnesses );
};

#endif // GEN_ALGO_HPP
