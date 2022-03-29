#include <iostream>
#include <vector>
#include <cstdlib>

#include "Network/environment.h"
#include "Network/parameterServer.h"
#include "Network/processingUnit.h"
#include "Network/event.h"


//Network::Stats createSim(int workers, int window, int CV, unsigned seed)
int main(){
    
    int iters = 10000;
    int n_Workers = 3;
    int window = 3;
    double CV = 2;

    //should be good enough for generating seeds
    std::srand(293687);

    //create controller and worker
    Network::ParameterServer* controller = new Network::ParameterServer(iters, window);//hard coded network delay 0.01

    std::vector<Network::ProcessingUnit*> workers;
    for (int i=0; i<n_Workers;i++){
        auto wkr = new Network::ProcessingUnit(CV,std::rand());
        workers.push_back(wkr);

        //link
        controller->connectWorker(wkr);
        wkr->connectController(controller);
    }

    //add initialization event
    Network::Environment* env = Network::Environment::getInstance();
    env->addEvent(Network::Event(0,0,0));

    //run
    env->runSim();

    std::cout<<"Simulation Complete\n";

    //change later for better interface
    Network::Stats s = controller->outputStats();
    std::cout<<"Throughput = "<<s.throughput<<std::endl;
    std::cout<<"Average Utilization = "<<s.avgUtilization<<std::endl;

    //clean up
    delete controller;
    for (int i=0; i<n_Workers;i++){
        delete workers[i];
    }

    return 0;
}