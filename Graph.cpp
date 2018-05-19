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

int Graph::getNbNodes( bool use_disabled ) const
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

int Graph::getMaxNode( bool use_disabled ) const
{
    int maxNode = 0;
    for ( const Connection& c : connections )
    {
        if ( use_disabled || c.enabled )
        {
            maxNode = std::max( std::max(
                        maxNode,
                        c.n0 ),
                        c.n1 );
        }
    }

    return maxNode;
}

std::vector<int> Graph::getLayers( bool use_disabled ) const
{
    // We are working on a treillis, where the minima are the input layer,
    // and the maxima are the output layers.
    int sz = getMaxNode(use_disabled)+1;
    std::vector<int> layers( sz, -1 );
    for ( int i = 0 ; i < nbInputs ; ++i )
        layers[i] = 0;

    // Compute layers forward
    bool changed = true;
    int lastLayer = 0;
    std::list<Connection> c_list( connections.begin(), connections.end() );
    while( changed )
    {
        changed = false;

        for ( auto it = c_list.begin() ; it != c_list.end() ; )
        {
            const Connection& c = *it;
            if ( (use_disabled||c.enabled) && layers[c.n0] != -1 && !isOutput(c.n1) )
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

Graph Graph::simplify( bool keep_disabled, int max_depth ) const
{
    Graph new_graph( nbInputs, nbOutputs, false );

    std::vector<bool> used_nodes( getMaxNode()+1, false );
    std::list<Connection> used_connections(
            connections.begin(), connections.end() );

    for ( int i = 0 ; i < nbInputs + nbOutputs ; ++i )
        used_nodes[i] = true;

    int current_depth = 0;

    // Browse the graph backward to see what node interacts with the outputs
    bool changed = true;
    while( changed && ( max_depth == -1 || current_depth < max_depth ) )
    {
        changed = false;
        for ( auto it = used_connections.begin() ;
                it != used_connections.end() ;
                /* manual incr */ )
        {
            if ( keep_disabled || it->enabled )
            {
                if ( used_nodes[it->n1] )
                {
                    if ( !used_nodes[it->n0] )
                    {
                        changed = true;
                        used_nodes[it->n0] = true;
                    }

                    new_graph.connections.push_back( *it );
                    it = used_connections.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }

        current_depth++;
    }

    // Construct node map
    std::map<int,int> node_map;
    int new_current_node = 0;
    for ( int i = 0 ; i < used_nodes.size() ; ++i )
    {
        if ( used_nodes[i] )
        {
            node_map[i] = new_current_node;
            new_current_node++;
        }
    }

    // Apply node map
    for ( Connection& c : new_graph.connections )
    {
        c.n0 = node_map[c.n0];
        c.n1 = node_map[c.n1];
    }

    return new_graph;
}
