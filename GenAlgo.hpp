#ifndef GEN_ALGO_HPP
#define GEN_ALGO_HPP

#include "Graph.hpp"
#include <random>

struct GenAlgo
{
    int population;

    // Mutation probabilities
    float pAddNode;
    float pAddConnection;
    float pChangeWeight;
    float pDisableConnection;
    float pEnableConnection;

    float defaultWeight;
    float dWeight; // Variation of weights during mutation/init
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
};

#endif // GEN_ALGO_HPP
