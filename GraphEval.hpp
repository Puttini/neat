#ifndef GRAPH_EVAL_SIGMOID
#define GRAPH_EVAL_SIGMOID

#include "Graph.hpp"
#include "types.hpp"
#include <map>

struct GraphEval
{
    GraphEval( const Graph& g );
    Vector<float> eval( const std::vector<float>& input, int nbIter = 1 );
    void reset();

    const Graph& graph;
    int nbInputs;
    int nbOutputs;
    int nbNodes;
    std::map<int,int> nodeMap;
    SpMat<float> adj;
    Vector<float> values;

    static float defaultValue;
    static float f( float x );
};

#endif // GRAPH_EVAL_SIGMOID
