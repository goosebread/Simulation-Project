#include "processingUnit.h"
#include "../Utils/logger.h"

namespace Network{

//legal functions for event handler to call
void ProcessingUnit::doEvent(int EventID){
    switch(EventID){
        case 1: 
            doPush();
            break;
        default:
            std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
    }
}

//schedule a completion when a task arrives
void ProcessingUnit::schedulePush(){
    auto env = Environment::getInstance();
    double serviceTime = gaussian(rng);
    Event task{env->getTime()+serviceTime, nodeID,1};
    env->addEvent(task);

    //log event
    Utils::Logger::getInstance()->file<<"worker scheduling a push event\n";
}

//gets run when its actually time to report a completed task
//bad hard coded names :(
void ProcessingUnit::doPush(){
    //log event
    Utils::Logger::getInstance()->file<<"worker pushing to PS\n";

    controller->processPush(getID());
}

//used to define the network
void ProcessingUnit::connectController(ParameterServer* unit){
    controller = unit;
}

void ProcessingUnit::processPull(){
    Utils::Logger::getInstance()->file<<"Worker pulling from PS\n";
    schedulePush();
}

}//end namespace