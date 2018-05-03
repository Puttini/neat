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

    Connection() = default;
    Connection( int n0, int n1, float w, int inno = 0 )
     : n0(n0), n1(n1), w(w), inno(inno)
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
    Graph( int nbInputs, int nbOutputs, bool init_connect = true );

    int getNbNodes() const;
    int getMaxNode() const;
    std::vector<int> getLayers() const;
    bool isInput( int n ) const;
    bool isOutput( int n ) const;
};

#endif // GRAPH_HPP
