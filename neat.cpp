#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include "types.hpp"
#include "Graph.hpp"

namespace py = pybind11;

PYBIND11_MODULE( neat, m )
{
    py::class_<Node> node( m, "Node" );
    node.def( py::init<>() );
    node.def( py::init<int>() );
    node.def( py::init<int,int>() );
    node.def_readwrite( "n", &Node::n );
    node.def_readwrite( "inno", &Node::inno );

    py::class_<Connection> connection( m, "Connection" );
    connection.def( py::init<>() );
    connection.def( py::init<int,int,float>() );
    connection.def( py::init<int,int,float,int>() );
    connection.def_readwrite( "n0", &Connection::n0 );
    connection.def_readwrite( "n1", &Connection::n1 );
    connection.def_readwrite( "w", &Connection::w );
    connection.def_readwrite( "inno", &Connection::inno );

    py::class_<Graph> graph( m, "Graph" );
    graph.def( py::init<>() );
    graph.def_readwrite( "nodes", &Graph::nodes );
    graph.def_readwrite( "connections", &Graph::connections );
}
