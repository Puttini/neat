#include "GenAlgo.hpp"

GenAlgo::GenAlgo( int nbInputs, int nbOutputs, int population )
 : population(population),

   // Default values
   pAddNode(0.03),
   pAddConnection(0.05),
   pChangeWeight(0.1),
   pDisableConnection(0.05),
   pEnableConnection(0.1),

   defaultWeight(0.5),
   dWeight(0.1),
   c12(1), c3(1),
   dThreshold(3)
{
    current_inno = 0;
    current_generation = 0;
    current_node = nbInputs + nbOutputs;

    genomes.reserve(population);

    for ( int p = 0 ; p < population ; ++p )
        genomes.emplace_back( nbInputs, nbOutputs );
}
