#include "GenAlgo.hpp"
#include <map>

GenAlgo::GenAlgo( int nbInputs, int nbOutputs, int population )
 : nbInputs(nbInputs),
   nbOutputs(nbOutputs),
   population(population),

   // Default values
   pAddNode(0.03),
   pAddConnection(0.05),
   pChangeWeight(0.1),
   pDisableConnection(0.01),
   pEnableConnection(0.05),

   // If this probability is < 1, a cross-over can have invalid nodes...
   pTakeNewGene(1.0),

   nbMaxTry(20),
   initStdDev(0.6),
   defStdDev(0.2),
   relStdDev(0.5),
   c12(1), c3(1),
   dThreshold(3)
{
    current_inno = nbInputs*nbOutputs; // Initial connections of genomes
    current_generation = 0;
    current_node = nbInputs + nbOutputs;

    genomes.reserve(population);

    for ( int p = 0 ; p < population ; ++p )
        genomes.emplace_back( nbInputs, nbOutputs );

    for ( Graph& g : genomes )
    {
        for ( Connection& c : g.connections )
            initWeight(c);
    }

    initSpecies();
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
        int cand_out = presentNodes[ out_dist(rng) ];

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
            initWeight( g.connections[ g.connections.size()-1 ] );
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

bool GenAlgo::initWeight( Connection& c )
{
    std::normal_distribution<float> dist( 0, initStdDev );
    c.w = dist(rng);
    return true;
}

Graph GenAlgo::crossOver(
        const Graph& g0, const Graph& g1,
        float fitness0, float fitness1 )
{
    Graph offspring( nbInputs, nbOutputs, false );

    std::uniform_real_distribution<float> choice(0,fitness0+fitness1);
    std::uniform_real_distribution<float> take_gene(0,1);

    // Construct the final adjacency matrix
    // The aim of that is to avoid several connections between the same points
    // (which is different from having different innovation numbers)
    int sz = std::max( g0.getMaxNode(), g1.getMaxNode() ) + 1;
    SpMat<Connection*> adj(sz,sz);

    int idx0 = 0;
    int idx1 = 0;
    while( idx0 < g0.connections.size() || idx1 < g1.connections.size() )
    {
        const Connection* c0 =
            idx0 < g0.connections.size()
            ? &g0.connections[idx0]
            : nullptr;
        const Connection* c1 =
            idx1 < g1.connections.size()
            ? &g1.connections[idx1]
            : nullptr;

        if ( c0 && c1 && c0->inno == c1->inno )
        {
            // Cross over of the same gene
            // There can't be overlapping connection here
            // as there is not for both children

            if ( choice(rng) < fitness0 )
                offspring.connections.push_back(*c0);
            else
                offspring.connections.push_back(*c1);

            Connection& new_c = offspring.connections[
                offspring.connections.size() -1 ];
            adj.insert( new_c.n0, new_c.n1 ) = &new_c;

            idx0++;
            idx1++;
        }
        else if ( c0 && ( !c1 || ( /*c1 &&*/ c0->inno < c1->inno ) ) )
        {
            // Take connection c0
            // but also test conflicts
            if ( adj.coeff(c0->n0,c0->n1) )
            {
                if ( choice(rng) < fitness0 )
                    *(adj.coeffRef(c0->n0,c0->n1)) = *c0;
            }
            else if ( take_gene(rng) < pTakeNewGene )
            {
                offspring.connections.push_back(*c0);
                Connection& new_c = offspring.connections[
                    offspring.connections.size() -1 ];
                adj.insert( new_c.n0, new_c.n1 ) = &new_c;
            }

            idx0++;
        }
        else
        {
            // Take connection c1
            // but also test conflicts
            if ( adj.coeff(c1->n0,c1->n1) )
            {
                if ( choice(rng) < fitness0 )
                    *(adj.coeffRef(c0->n0,c0->n1)) = *c0;
            }
            else if ( take_gene(rng) < pTakeNewGene )
            {
                offspring.connections.push_back(*c1);
                Connection& new_c = offspring.connections[
                    offspring.connections.size() -1 ];
                adj.insert( new_c.n0, new_c.n1 ) = &new_c;
            }

            idx1++;
        }
    }

    return offspring;
}

float GenAlgo::computeCompDist(
        const Graph& g0,
        const Graph& g1 ) const
{
    return computeCompDist(g0,g1,getNbMaxGenes());
}

float GenAlgo::computeCompDist(
        const Graph& g0,
        const Graph& g1,
        int nbMaxGenes ) const
{
    int idx0 = 0;
    int idx1 = 0;
    float dist = 0;
    float coeff12 = static_cast<float>(c12) / static_cast<float>(nbMaxGenes);
    while( idx0 < g0.connections.size() || idx1 < g1.connections.size() )
    {
        const Connection& c0 = g0.connections[idx0];
        const Connection& c1 = g1.connections[idx1];

        if ( c0.inno == c1.inno )
        {
            float w0 = c0.enabled ? c0.w : 0;
            float w1 = c1.enabled ? c1.w : 0;
            dist += coeff12 * std::abs(w0-w1);

            idx0++;
            idx1++;
        }
        else if ( c0.inno < c1.inno )
        {
            dist += coeff12;
            idx0++;
        }
        else
        {
            dist += coeff12;
            idx1++;
        }
    }

    return dist;
}

int GenAlgo::getNbMaxGenes() const
{
    int nb = 0;
    for ( const Graph& g : genomes )
        nb = std::max<int>( nb, g.connections.size() );
    return nb;
}

void GenAlgo::initSpecies()
{
    speciesPerGenome.clear();
    speciesRepresentants.clear();
    popPerSpecies.clear();

    speciesPerGenome.resize( genomes.size() );

    int nbMaxGenes = getNbMaxGenes();
    for ( int g = 0 ; g < genomes.size() ; ++g )
    {
        bool foundSpecies = false;
        for ( int s = 0 ; s < speciesRepresentants.size() ; ++s )
        {
            if ( computeCompDist(
                        genomes[g],
                        genomes[ speciesRepresentants[s] ],
                        nbMaxGenes ) < dThreshold )
            {
                foundSpecies = true;
                speciesPerGenome[g] = s;
                popPerSpecies[s]++;
                break;
            }
        }
        if ( !foundSpecies )
        {
            speciesPerGenome[g] = speciesRepresentants.size();
            popPerSpecies.push_back(1);
            speciesRepresentants.push_back(g);
        }
    }
}

void GenAlgo::actualizeSpecies()
{
    std::vector<int> newSpeciesPerGenome;
    std::vector<int> newSpeciesRepresentants;
    std::vector<int> newPopPerSpecies;
    newSpeciesPerGenome.resize( genomes.size() );

    std::map<int,int> speciesMap;

    int nbMaxGenes = getNbMaxGenes();

    // Only look for old species
    for ( int g = 0 ; g < genomes.size() ; ++g )
    {
        bool foundSpecies = false;
        for ( int s = 0 ; s < speciesRepresentants.size() ; ++s )
        {
            if ( computeCompDist(
                        genomes[g],
                        genomes[ speciesRepresentants[s] ],
                        nbMaxGenes ) < dThreshold )
            {
                auto match = speciesMap.find(s);
                if ( match == speciesMap.end() )
                {
                    // First genome matching this old species
                    int newSpecies = newSpeciesRepresentants.size();
                    newSpeciesPerGenome[g] = newSpecies;
                    newSpeciesRepresentants.push_back(g);
                    newPopPerSpecies.push_back(1);
                    speciesMap[s] = newSpecies;
                }
                else
                {
                    // Old species already matched
                    int species = match->second;
                    newSpeciesPerGenome[g] = species;
                    newPopPerSpecies[species]++;
                }
                foundSpecies = true;
                break;
            }
        }
        if ( !foundSpecies )
        {
            newSpeciesPerGenome[g] = -1;
        }
    }

    // Then construct the new species for genomes that are not speciated yet
    int nbOldSpecies = newSpeciesRepresentants.size();
    for ( int g = 0 ; g < genomes.size() ; ++g )
    {
        if ( newSpeciesPerGenome[g] == -1 )
        {
            bool foundSpecies = false;
            for ( int s = nbOldSpecies ; s < newSpeciesRepresentants.size() ; ++s )
            {
                if ( computeCompDist(
                            genomes[g],
                            genomes[ newSpeciesRepresentants[s] ],
                            nbMaxGenes ) < dThreshold )
                {
                    foundSpecies = true;
                    newSpeciesPerGenome[g] = s;
                    newPopPerSpecies[s]++;
                    break;
                }
            }
            if ( !foundSpecies )
            {
                newSpeciesPerGenome[g] = newSpeciesRepresentants.size();
                newPopPerSpecies.push_back(1);
                newSpeciesRepresentants.push_back(g);
            }
        }
    }

    // Finally swap the vectors (faster than copy)
    speciesPerGenome.swap( newSpeciesPerGenome );
    speciesRepresentants.swap( newSpeciesRepresentants );
    popPerSpecies.swap( newPopPerSpecies );
}
