#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include <iostream>

#include "types.hpp"
#include "Graph.hpp"

namespace py = pybind11;

PYBIND11_MODULE( neat, m )
{
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
    connection.def( "__repr__", []( const Connection& c )
            {
                std::stringstream ss;
                ss << "<| Connection: " << c.n0 << " -> " << c.n1 << ", "
                   << "w=" << c.w << ", inno = " << c.inno << " |>";
                return ss.str();
            });

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
}
