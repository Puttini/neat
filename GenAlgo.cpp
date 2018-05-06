#include "GenAlgo.hpp"
#include <map>

GenAlgo::GenAlgo( int nbInputs, int nbOutputs, int population )
 : population(population),

   // Default values
   pAddNode(0.03),
   pAddConnection(0.05),
   pChangeWeight(0.1),
   pDisableConnection(0.01),
   pEnableConnection(0.05),

   nbMaxTry(20),
   defStdDev(0.5),
   relStdDev(0.5),
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

    int cn = dist(rng);
    int nbTry = 0;
    while ( !g.connections[cn].enabled && nbTry < nbMaxTry )
    {
        cn = dist(rng);
        nbTry++;
    }
    if ( nbTry >= nbMaxTry )
        return false;

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
    // List present nodes in the graph
    std::map<int,int> presentNodes;
    int nbNodes = 0;
    for ( const Connection& c : g.connections )
    {
        if ( presentNodes.find( c.n0 ) == presentNodes.end() )
        {
            presentNodes[nbNodes] = c.n0;
            nbNodes++;
        }
        if ( presentNodes.find( c.n1 ) == presentNodes.end() )
        {
            presentNodes[nbNodes] = c.n1;
            nbNodes++;
        }
    }

    // An output of a node cannot be an input of the network,
    // but an input can be an output of the network
    std::uniform_int_distribution<int> in_dist(0, nbNodes-1);
    std::uniform_int_distribution<int> out_dist(g.nbInputs, nbNodes-1);
    for ( int nbTry = 0 ; nbTry < nbMaxTry ; ++nbTry )
    {
        int cand_in = presentNodes[ in_dist(rng) ];
        int cand_out = presentNodes[ in_dist(rng) ];

        // Test if the connection already exists in the graph
        bool exists = false;
        for ( const Connection& c : g.connections )
        {
            if ( c.n0 == cand_in && c.n1 == cand_out )
            {
                exists = true;
                break;
            }
        }

        // If the new connection is correct, then just add it with a
        // random weight around 0, and return true
        // Otherwise, try again
        if ( exists )
        {
            continue;
        }
        else
        {
            g.connections.emplace_back( cand_in, cand_out, 0, current_inno );
            current_inno++;
            changeWeight( g.connections[ g.connections.size()-1 ] );
            return true;
        }
    }

    return false;
}

bool GenAlgo::changeWeight( Connection& c )
{
    std::normal_distribution<float> dist( c.w,
            defStdDev + relStdDev*std::abs(c.w) );
    c.w = dist(rng);
    return true;
}
