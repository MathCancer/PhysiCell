#include <omp.h>
#include <stdio.h>
#include <iostream>
#include "StochasticSimulationEngine.h"
#include "BooleanNetwork.h"
#include "RunConfig.h"

int main ( int argc, char *argv[] )
{
    try{

        Network* network = new Network();
        network->parse("../examples/openmp/test2.bnd");

        RunConfig* config = new RunConfig();
        config->parse(network, "../examples/openmp/test2.cfg");

        IStateGroup::checkAndComplete(network);
    
        Network* network_mut = new Network();
        network_mut->parse("../examples/openmp/test2.bnd");

        RunConfig* config_mut = new RunConfig();
        config_mut->parse(network_mut, "../examples/openmp/test2_mutant.cfg");

        IStateGroup::checkAndComplete(network_mut);

#pragma omp parallel
{
        StochasticSimulationEngine* engine = new StochasticSimulationEngine(network, config);
        int seed = config->getSeedPseudoRandom() + omp_get_thread_num();
        engine->setSeed(seed);

        // First argument of run is a pointer to the initial state. 
        // If NULL, it will use initial states as defined by the cfg
        NetworkState_Impl state = engine->run(NULL, NULL);
        NetworkState_Impl next_state = engine->run(&state, NULL);

        StochasticSimulationEngine* engine_mut = new StochasticSimulationEngine(network_mut, config_mut);
        int seed_mut = config_mut->getSeedPseudoRandom() + omp_get_thread_num();
        engine_mut->setSeed(seed_mut);

        NetworkState_Impl state_mut = engine_mut->run(NULL, NULL);
        NetworkState_Impl next_state_mut = engine_mut->run(&state_mut, NULL);

    #pragma omp critical
    {
        std::cout << "WT : ";
        ((NetworkState)state).displayOneLine(std::cout, network);
        std::cout << "  -->  ";
        ((NetworkState)next_state).displayOneLine(std::cout, network);
        std::cout << std::endl;

        std::cout << "Mutant : ";
        ((NetworkState)state_mut).displayOneLine(std::cout, network_mut);
        std::cout << "  -->  ";
        ((NetworkState)next_state_mut).displayOneLine(std::cout, network_mut);
        std::cout << std::endl;
    }
        delete engine;
        delete engine_mut;

}   
        delete config;
        delete network;
        delete config_mut;
        delete network_mut;
    }
    catch (BNException e) {
        std::cerr << e.getMessage() << std::endl;
    }
    
    return 0;
}