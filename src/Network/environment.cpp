#include "environment.h"
#include "../Utils/logger.h"
namespace Network{

//initialize static variable
Environment* Environment::instance = nullptr;

Environment::Environment(){
    nodes = std::vector<Node*>();
}

//main simulation loop
void Environment::runSim(){
    currentTime=0;
    //loop until events queue is empty
    while(!events.empty()){
        Event current = events.top();
        events.pop();

        //log data
        auto logger = Utils::Logger::getInstance();
        logger->file<<"time = "<<current.time<<" node = "<<current.NodeID<<" event = "<<current.EventID<<std::endl;

        //run the event
        currentTime = current.time;
        nodes.at(current.NodeID)->doEvent(current.EventID);

        //could optimize and distribute the update responsibility to nodes 
        //and only call for nodes that actually change
        for (auto node : nodes){
            node->updateStats();
        }
    }

    //flush buffer before file closes
    Utils::Logger::getInstance()->file<<std::flush;
}

}//end namespace network

