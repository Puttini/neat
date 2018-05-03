#include "Graph.hpp"
#include <set>

Graph::Graph( int nbInputs, int nbOutputs, bool init_connect )
 : nbInputs(nbInputs), nbOutputs(nbOutputs)
{
    int i = 0;

    if ( init_connect )
    {
        connections.reserve( nbInputs * nbOutputs );
        for ( int j = 0 ; j < nbInputs ; ++j )
        {
            for ( int k = 0 ; k < nbOutputs ; ++k )
            {
                connections.emplace_back( j, nbInputs + k, 1.f, i );
                ++i;
            }
        }
    }
}

int Graph::getNbNodes() const
{
    std::set<int> nodes;
    for ( const Connection& c : connections )
    {
        nodes.insert( c.n0 );
        nodes.insert( c.n1 );
    }

    return nodes.size();
}

int Graph::getMaxNode() const
{
    int maxNode = 0;
    for ( const Connection& c : connections )
        maxNode = std::max( std::max(
                    maxNode,
                    c.n0 ),
                    c.n1 );

    return maxNode;
}

std::vector<int> Graph::getLayers() const
{
    // We are working on a treillis, where the minima are the input layer,
    // and the maxima are the output layers.
    int maxNode = getMaxNode();
    std::vector<int> layers( getMaxNode() + 1, -1 );
    for ( int i = 0 ; i < nbInputs ; ++i )
        layers[i] = 0;

    bool changed = true;
    int lastLayer = 0;
    while( changed )
    {
        changed = false;

        for ( const Connection& c : connections )
        {
            if ( layers[c.n0] != -1 )
            {
                int n1layer = layers[c.n0] + 1;
                if ( n1layer > layers[c.n1] )
                {
                    layers[c.n1] = n1layer;
                    changed = true;
                    lastLayer = std::max( lastLayer, n1layer );
                }
            }
        }
    }

    // Correct outputs
    for ( int i = nbInputs ; i < nbInputs + nbOutputs ; ++i )
        layers[i] = lastLayer;

    return layers;
}

bool Graph::isInput( int n ) const
{
    return n < nbInputs;
}

bool Graph::isOutput( int n ) const
{
    return n >= nbInputs && n < nbInputs + nbOutputs;
}

