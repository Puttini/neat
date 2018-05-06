#include "GenAlgo.hpp"

GenAlgo::GenAlgo( int nbInputs, int nbOutputs, int population )
 : population(population),

   // Default values
   pAddNode(0.03),
   pAddConnection(0.05),
   pChangeWeight(0.1),
   pDisableConnection(0.01),
   pEnableConnection(0.05),

   defaultWeight(0.5),
   dWeight(0.1),
   c12(1), c3(1),
   dThreshold(3)
{
    current_inno = 0;
    current_generation = 0;
    current_node = nbInputs + nbOutputs;

    genomes.reserve(population);

    for ( int p = 0 ; p < population ; ++p )
        genomes.emplace_back( nbInputs, nbOutputs );
}

bool GenAlgo::mutate_all()
{
    bool mutated = false;
    for ( Graph& g : genomes )
    {
        if ( mutate(g) )
            mutated = true;
    }

    return mutated;
}

bool GenAlgo::mutate( Graph& g )
{
    std::uniform_real_distribution<float> dist(0,1);
    bool mutated = false;

    if ( dist(rng) < pAddNode )
    {
        if ( addNode(g) )
            mutated = true;
    }

    if ( dist(rng) < pAddConnection )
    {
        if ( addConnection(g) )
            mutated = true;
    }

    for ( Connection& c : g.connections )
    {
        if ( c.enabled )
        {
            if ( dist(rng) < pDisableConnection )
            {
                c.enabled = false;
                mutated = true;
            }
        }
        else
        {
            if ( dist(rng) < pEnableConnection )
            {
                c.enabled = true;
                mutated = true;
            }
        }

        if ( dist(rng) < pChangeWeight )
            changeWeight(c);
    }

    return mutated;
}

void GenAlgo::setSeed( int seed )
{
    rng.seed(seed);
}

bool GenAlgo::addNode( Graph& g )
{
    std::uniform_int_distribution<int> dist(0,g.connections.size()-1);

    // The graph is always connected by construction
    // and should mainly contain enabled connections,
    // so this loop should be fast and correct
    int cn = dist(rng);
    while ( !g.connections[cn].enabled )
        cn = dist(rng);

    // Split this connection into 2 connections
    g.connections[cn].enabled = false;

    g.connections.emplace_back(
            g.connections[cn].n0,
            current_node,
            g.connections[cn].w,
            current_inno );
    current_inno++;
    g.connections.emplace_back(
            current_node,
            g.connections[cn].n1,
            1,
            current_inno );
    current_inno++;
    current_node++;

    return true;
}

bool GenAlgo::addConnection( Graph& g )
{
    return false;
}

bool GenAlgo::changeWeight( Connection& c )
{
    return false;
}
