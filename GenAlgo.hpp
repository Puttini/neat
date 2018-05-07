#ifndef GEN_ALGO_HPP
#define GEN_ALGO_HPP

#include "Graph.hpp"
#include <random>

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

    int nbMaxTry;
    float initStdDev;
    float defStdDev;
    float relStdDev;
    float c12, c3;
    float dThreshold;

    int current_inno;
    int current_node; // Next node to be added to the graph
    int current_generation;

    std::vector<Graph> genomes;

    std::default_random_engine rng;

// ---------------------------------------------------------------------------

    GenAlgo() = default;
    GenAlgo( int nbInputs, int nbOutputs, int population = 150 );

    void setSeed( int seed );

    bool mutate_all();
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
            const Graph& g0, const Graph& g1 ) const;
    float computeCompDist(
            const Graph& g0, const Graph& g1,
            int nbMaxGenes ) const;
};

#endif // GEN_ALGO_HPP
