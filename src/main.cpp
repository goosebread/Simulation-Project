#include <iostream>
#include "Network/environment.h"
#include "Network/parameterServer.h"
#include "Network/processingUnit.h"
#include "Network/event.h"

int main(){

    //create controller and worker
    Network::ParameterServer* controller = new Network::ParameterServer();
    Network::ProcessingUnit* worker1 = new Network::ProcessingUnit();
    Network::ProcessingUnit* worker2 = new Network::ProcessingUnit();

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

    std::cout<<"hello!";
    return 0;
}