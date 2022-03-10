#include "processingUnit.h"

namespace Network{

//legal functions for event handler to call
void ProcessingUnit::doEvent(int EventID){
    switch(EventID){
        case 1: 
            doCompletion();
            break;
        default:
            std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
    }
}

//schedule a completion when a task arrives
void ProcessingUnit::scheduleCompletion(){
    auto env = Environment::getInstance();
    Event task{env->getTime()+serviceTime, nodeID,1};
    env->addEvent(task);

    //log event
    std::cout<<"worker scheduling a completion event\n";
}

//gets run when its actually time to report a completed task
//bad hard coded names :(
void ProcessingUnit::doCompletion(){
    //log event
    std::cout<<"worker reporting a completion event to PS\n";

    controller->processCompletion();
}

//used to define the network
void ProcessingUnit::connectController(ParameterServer* unit){
    controller = unit;
}

}//end namespace