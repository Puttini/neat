#include "GraphEval.hpp"

float GraphEval::defaultValue = 0.5f;

// static
float GraphEval::f( float x )
{
    return static_cast<float>(std::tanh(x))/2.f + 0.5f;
}

GraphEval::GraphEval( const Graph& g )
 : graph(g)
{
    // Remove useless nodes
    nbInputs = g.nbInputs;
    nbOutputs = g.nbOutputs;

    nbNodes = nbInputs + nbOutputs;
    for ( int n = 0 ; n < g.nbInputs + g.nbOutputs ; ++n )
        nodeMap[n] = n;

    // Only keep nodes linked to the output
    bool changed = true;
    int nbMaxNode = g.getMaxNode(false);
    while ( changed )
    {
        changed = false;
        for ( const Connection& c : g.connections )
        {
            if ( c.enabled
                 && nodeMap.find(c.n1) != nodeMap.end()
                 && nodeMap.find(c.n0) == nodeMap.end() )
            {
                nodeMap[c.n0] = nbNodes;
                nbNodes++;
                changed = true;
            }
        }
    }

    // Construct adjacency matrix
    adj.resize( nbNodes, nbNodes );
    for ( const Connection& c : g.connections )
    {
        if ( c.enabled )
        {
            auto match0 = nodeMap.find(c.n0);
            auto match1 = nodeMap.find(c.n1);
            if ( match0 != nodeMap.end() && match1 != nodeMap.end() )
                adj.insert(match1->second,match0->second) = c.w;
        }
    }
    adj.makeCompressed();

    values.resize( nbNodes );
    values.setConstant( defaultValue );
}

Vector<float> GraphEval::evaluate( const std::vector<float>& input, int nbIter )
{
    Eigen::Map< const Vector<float> > v_input( input.data(), nbInputs );
    values.head(nbInputs) = v_input;

    for ( int i = 0 ; i < nbIter ; ++i )
    {
        values = adj * values;

        for ( int n = nbInputs ; n < nbNodes ; ++n )
            values(n) = f(values(n));
    }

    return values.segment(nbInputs,nbOutputs);
}
