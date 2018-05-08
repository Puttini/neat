PROJECT( neat )

CMAKE_MINIMUM_REQUIRED( VERSION 3.1 )

add_subdirectory( "pybind11" )
include_directories( "eigen" )

add_library( source STATIC
    Graph.cpp
    GenAlgo.cpp )
set_target_properties( source PROPERTIES COMPILE_FLAGS -fPIC )

pybind11_add_module( neat neat.cpp )
target_link_libraries( neat PRIVATE source )

add_executable( test test.cpp )
target_link_libraries( test PRIVATE source )
