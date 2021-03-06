#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "types.hpp"
#include <vector>

// n0 -> n1
struct Connection
{
    int n0;
    int n1;
    float w;
    int inno;

    bool enabled;

    Connection() = default;
    Connection( int n0, int n1, float w, int inno = 0, bool enabled=true )
     : n0(n0), n1(n1), w(w), inno(inno), enabled(enabled)
    {}
};

// A graph shall always have its inputs and outputs at the beginning
// of its Node vector, with nodes[i].n = i
struct Graph
{
    std::vector<Connection> connections;
    int nbInputs;
    int nbOutputs;


    Graph() = default;
    Graph( const Graph& ) = default;
    Graph( int nbInputs, int nbOutputs, bool init_connect = true );

    int getNbNodes( bool use_disabled = true ) const;
    int getMaxNode( bool use_disabled = true ) const;
    std::vector<int> getLayers( bool use_disabled = true ) const;
    SpMat<const Connection*> getAdjacencyMatrix() const;
    std::vector< std::pair<int,int> > getNbConnectionsPerNode() const;
    bool isInput( int n ) const;
    bool isOutput( int n ) const;
    Graph simplify( bool keep_disabled = false, int max_depth = -1 ) const;
};

#endif // GRAPH_HPP
