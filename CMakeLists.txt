PROJECT( neat )

CMAKE_MINIMUM_REQUIRED( VERSION 3.1 )

add_subdirectory( "pybind11" )
include_directories( "eigen" )

pybind11_add_module( neat neat.cpp )
