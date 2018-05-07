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
                connections.emplace_back( j, nbInputs + k, 0.f, i );
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
        if ( c.enabled )
        {
            nodes.insert( c.n0 );
            nodes.insert( c.n1 );
        }
    }

    return nodes.size();
}

int Graph::getMaxNode() const
{
    int maxNode = 0;
    for ( const Connection& c : connections )
    {
        if ( c.enabled )
        {
            maxNode = std::max( std::max(
                        maxNode,
                        c.n0 ),
                        c.n1 );
        }
    }

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
    std::list<Connection> c_list( connections.begin(), connections.end() );
    while( changed )
    {
        changed = false;

        for ( auto it = c_list.begin() ; it != c_list.end() ; )
        {
            const Connection& c = *it;
            if ( c.enabled && layers[c.n0] != -1 && !isOutput(c.n1) )
            {
                int n1layer = layers[c.n0] + 1;
                if ( n1layer > layers[c.n1] )
                {
                    assert( n1layer < connections.size() );
                    layers[c.n1] = n1layer;
                    changed = true;
                    lastLayer = std::max( lastLayer, n1layer );
                }

                it = c_list.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    lastLayer++;

    // Correct inputs and outputs
    int i = 0;
    for ( ; i < nbInputs ; ++i )
        layers[i] = 0;
    for ( ; i < nbInputs + nbOutputs ; ++i )
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

SpMat<const Connection*> Graph::getAdjacencyMatrix() const
{
    int sz = getMaxNode() + 1;
    SpMat<const Connection*> adj( sz, sz );
    for ( const Connection& c : connections )
        adj.insert(c.n0,c.n1) = &c;
    adj.makeCompressed();
    return adj;
}

std::vector< std::pair<int,int> > Graph::getNbConnectionsPerNode() const
{
    int sz = getMaxNode() + 1;
    std::vector< std::pair<int,int> > nbConnections(sz);
    for ( auto& p : nbConnections )
    {
        p.first = 0;
        p.second = 0;
    }

    for ( const Connection& c : connections )
    {
        nbConnections[c.n0].second++;
        nbConnections[c.n1].first++;
    }

    return nbConnections;
}
