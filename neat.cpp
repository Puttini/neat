#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include <iostream>

#include "types.hpp"
#include "Graph.hpp"
#include "GenAlgo.hpp"
#include "GraphEval.hpp"

namespace py = pybind11;

PYBIND11_MODULE( neat, m )
{
    // --- Connection ---
    py::class_<Connection> connection( m, "Connection" );
    connection.def( py::init<>() );
    connection.def( py::init<int,int,float,int>(),
            py::arg( "n0" ),
            py::arg( "n1" ),
            py::arg( "w" ) = 0,
            py::arg( "inno" ) = 0);
    connection.def_readwrite( "n0", &Connection::n0 );
    connection.def_readwrite( "n1", &Connection::n1 );
    connection.def_readwrite( "w", &Connection::w );
    connection.def_readwrite( "inno", &Connection::inno );
    connection.def_readwrite( "enabled", &Connection::enabled );
    connection.def( "__repr__", []( const Connection& c )
            {
                std::stringstream ss;
                ss << "<| Connection: ";
                if ( !c.enabled )
                    ss << "DIS ";
                ss << c.n0 << " -> " << c.n1 << ", "
                   << "w=" << c.w << ", inno = " << c.inno << " |>";
                return ss.str();
            });

    // --- Graph ---
    py::class_<Graph> graph( m, "Graph" );
    graph.def( py::init<>() );
    graph.def( py::init<int,int,bool>(),
            py::arg( "nbInputs" ),
            py::arg( "nbOutputs" ),
            py::arg( "init_connect" ) = true );
    graph.def( py::init<const Graph&>() );
    graph.def_readwrite( "connections", &Graph::connections );
    graph.def_readwrite( "nbInputs", &Graph::nbInputs );
    graph.def_readwrite( "nbOutputs", &Graph::nbOutputs );
    graph.def( "getNbNodes", &Graph::getNbNodes,
               py::arg( "use_disabled" ) = true );
    graph.def( "getLayers", &Graph::getLayers,
               py::arg( "use_disabled" ) = true );
    graph.def( "getMaxNode", &Graph::getMaxNode,
               py::arg( "use_disabled" ) = true );
    graph.def( "getAdjacencyMatrix", &Graph::getAdjacencyMatrix );
    graph.def( "getNbConnectionsPerNode", &Graph::getNbConnectionsPerNode );
    graph.def( "isInput", &Graph::isInput );
    graph.def( "isOutput", &Graph::isOutput );

    // --- GenAlgo ---
    py::class_<GenAlgo> genalgo( m, "GenAlgo" );
    genalgo.def( py::init<>() );
    genalgo.def( py::init<int,int,int>(),
            py::arg( "nbInputs" ),
            py::arg( "nbOutputs" ),
            py::arg( "population" ) = 150 );
    genalgo.def_readonly( "population", &GenAlgo::population );
    genalgo.def_readwrite( "pAddNode", &GenAlgo::pAddNode );
    genalgo.def_readwrite( "pAddConnection", &GenAlgo::pAddConnection );
    genalgo.def_readwrite( "pChangeWeight", &GenAlgo::pChangeWeight );
    genalgo.def_readwrite( "pDisableConnection", &GenAlgo::pDisableConnection );
    genalgo.def_readwrite( "pEnableConnection", &GenAlgo::pEnableConnection );
    genalgo.def_readwrite( "pTakeNewGene", &GenAlgo::pTakeNewGene );
    genalgo.def_readwrite( "nbSurvivors", &GenAlgo::nbSurvivors );
    genalgo.def_readwrite( "nbChildren", &GenAlgo::nbChildren );
    genalgo.def_readwrite( "interSpeciesRate", &GenAlgo::interSpeciesRate );
    genalgo.def_readwrite( "nbMaxTry", &GenAlgo::nbMaxTry );
    genalgo.def_readwrite( "initStdDev", &GenAlgo::initStdDev );
    genalgo.def_readwrite( "defStdDev", &GenAlgo::defStdDev );
    genalgo.def_readwrite( "relStdDev", &GenAlgo::relStdDev );
    genalgo.def_readwrite( "c12", &GenAlgo::c12 );
    genalgo.def_readwrite( "c3", &GenAlgo::c3 );
    genalgo.def_readwrite( "dThreshold", &GenAlgo::dThreshold );
    genalgo.def_readonly( "current_inno", &GenAlgo::current_inno );
    genalgo.def_readonly( "current_node", &GenAlgo::current_node );
    genalgo.def_readonly( "current_generation", &GenAlgo::current_generation );
    genalgo.def_readwrite( "genomes", &GenAlgo::genomes );
    genalgo.def_readonly( "speciesPerGenome", &GenAlgo::speciesPerGenome );
    genalgo.def_readonly( "speciesRepresentants", &GenAlgo::speciesRepresentants );
    genalgo.def_readonly( "popPerSpecies", &GenAlgo::popPerSpecies );

    genalgo.def( "setSeed", &GenAlgo::setSeed );
    genalgo.def( "mutate_all", &GenAlgo::mutate_all );
    genalgo.def( "mutate", &GenAlgo::mutate );
    genalgo.def( "addNode", &GenAlgo::addNode );
    genalgo.def( "addConnection", &GenAlgo::addConnection );
    genalgo.def( "changeWeight", &GenAlgo::changeWeight );
    genalgo.def( "initWeight", &GenAlgo::changeWeight );
    genalgo.def( "crossOver", &GenAlgo::crossOver );
    genalgo.def( "getNbMaxGenes", &GenAlgo::getNbMaxGenes );
    genalgo.def( "computeCompDist", &GenAlgo::computeCompDist );
    genalgo.def( "init", &GenAlgo::init );
    genalgo.def( "initSpecies", &GenAlgo::initSpecies );
    genalgo.def( "actualizeSpecies", &GenAlgo::actualizeSpecies );
    genalgo.def( "nextGen", &GenAlgo::nextGen );

    // --- GraphEval ---
    py::class_<GraphEval> ge( m, "GraphEval" );
    ge.def( py::init<const Graph&>() );
    ge.def( "eval", &GraphEval::eval,
            py::arg( "input" ),
            py::arg( "nbIter" ) = 1 );
    ge.def( "reset", &GraphEval::reset );
    ge.def_readonly( "nbInputs", &GraphEval::nbInputs );
    ge.def_readonly( "nbOutputs", &GraphEval::nbOutputs );
    ge.def_readonly( "nbNodes", &GraphEval::nbNodes );
    ge.def_readonly( "adj", &GraphEval::adj );
    ge.def_readonly( "values", &GraphEval::values );
    ge.def_readonly( "nodeMap", &GraphEval::nodeMap );
    ge.def( "graph", []( const GraphEval& ge ){ return ge.graph; } );
}
