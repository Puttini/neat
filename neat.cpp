#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include <iostream>

#include "types.hpp"
#include "Graph.hpp"
#include "GenAlgo.hpp"

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
                ss << "<| Connection: " << c.n0 << " -> " << c.n1 << ", "
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
    graph.def_readwrite( "connections", &Graph::connections );
    graph.def_readwrite( "nbInputs", &Graph::nbInputs );
    graph.def_readwrite( "nbOutputs", &Graph::nbOutputs );
    graph.def( "getNbNodes", &Graph::getNbNodes );
    graph.def( "getLayers", &Graph::getLayers );
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
    genalgo.def_readwrite( "defaultWeight", &GenAlgo::defaultWeight );
    genalgo.def_readwrite( "dWeight", &GenAlgo::dWeight );
    genalgo.def_readwrite( "c12", &GenAlgo::c12 );
    genalgo.def_readwrite( "c3", &GenAlgo::c3 );
    genalgo.def_readwrite( "dThreshold", &GenAlgo::dThreshold );
    genalgo.def_readonly( "current_inno", &GenAlgo::current_inno );
    genalgo.def_readonly( "current_node", &GenAlgo::current_node );
    genalgo.def_readonly( "current_generation", &GenAlgo::current_generation );
    genalgo.def_readwrite( "genomes", &GenAlgo::genomes );

    genalgo.def( "setSeed", &GenAlgo::setSeed );
    genalgo.def( "mutate_all", &GenAlgo::mutate_all );
    genalgo.def( "mutate", &GenAlgo::mutate );
    genalgo.def( "addNode", &GenAlgo::addNode );
    genalgo.def( "addConnection", &GenAlgo::addConnection );
    genalgo.def( "changeWeight", &GenAlgo::changeWeight );
}
