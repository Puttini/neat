#include "GenAlgo.hpp"

int main( int argc, char** argv )
{
    GenAlgo ga( 2, 2, 150 );

    std::vector<float> fitnesses( 150, 1.f );
    for ( int i = 0 ; i < 150 ; ++i )
        ga.nextGen( fitnesses );

    return 0;
}
