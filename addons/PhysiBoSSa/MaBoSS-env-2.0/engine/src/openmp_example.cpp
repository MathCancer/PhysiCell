#include <omp.h>
#include <stdio.h>
#include <iostream>
#include "StochasticSimulationEngine.h"
#include "BooleanNetwork.h"
#include "RunConfig.h"

int omp_get_thread_num();

int main ( int argc, char *argv[] )
{
    try{

        Network* network = new Network();
        network->parse("../examples/openmp/test.bnet");

        RunConfig* config = new RunConfig();
        config->parse(network, "../examples/openmp/test.cfg");

        IStateGroup::checkAndComplete(network);
    

#pragma omp parallel
{
        StochasticSimulationEngine* engine = new StochasticSimulationEngine(network, config);
        int seed = config->getSeedPseudoRandom() + omp_get_thread_num();
        engine->setSeed(seed);
        
        NetworkState_Impl state = engine->run(NULL, NULL);
        NetworkState_Impl next_state = engine->run(&state, NULL);

    #pragma omp critical
    {
        ((NetworkState)state).displayOneLine(std::cout, network);
        std::cout << "  -->  ";
        ((NetworkState)next_state).displayOneLine(std::cout, network);
        std::cout << std::endl;
    }
        delete engine;
}       
        delete config;
        delete network;

    }
    catch (BNException e) {
        std::cerr << e.getMessage() << std::endl;
    }
    
    return 0;
}