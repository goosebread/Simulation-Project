#include "environment.h"
#include "../Utils/logger.h"
namespace Network{

//initialize static variable
Environment* Environment::instance = nullptr;

//probably unneccessary, could try deleting later
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
        //nodeID are unique and start from zero anyways
        //(*logger)("time = ");(*logger)(current.time);//logger->(" node = ");logger(current.NodeID);
        //logger(" event = ");logger(current.EventID);logger("\n");
        logger->file<<"time = "<<current.time<<" node = "<<current.NodeID<<" event = "<<current.EventID<<std::endl;

        //run the event
        currentTime = current.time;
        nodes.at(current.NodeID)->doEvent(current.EventID);
    }
}

}//end namespace network

