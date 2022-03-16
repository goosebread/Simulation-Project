#include <iostream>
#include "Network/environment.h"
#include "Network/parameterServer.h"
#include "Network/processingUnit.h"
#include "Network/event.h"

int main(){

    //create controller and worker
    Network::ParameterServer* controller = new Network::ParameterServer(2,0,191349);
    Network::ProcessingUnit* worker1 = new Network::ProcessingUnit(2,0.3,36857);
    Network::ProcessingUnit* worker2 = new Network::ProcessingUnit(3,0.4,239876);

    //link
    controller->connectWorker(worker1);
    worker1->connectController(controller);

    controller->connectWorker(worker2);
    worker2->connectController(controller);

    //add initialization event
    Network::Environment* env = Network::Environment::getInstance();
    env->addEvent(Network::Event(0,0,0));

    //run
    env->runSim();

    std::cout<<"Simulation Complete\n";
    return 0;
}