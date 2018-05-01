#include "types.hpp"

struct Node
{
    int n;
    int inno;

    Node() = default;
    Node( int n, int inno = 0 )
     : n(n), inno(inno)
    {}
};

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

struct Graph
{
    std::vector<Node> nodes;
    std::vector<Connection> connections;
};
