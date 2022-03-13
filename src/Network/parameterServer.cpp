#include "parameterServer.h"

namespace Network{

//legal functions for event handler to call
void ParameterServer::doEvent(int EventID){
    switch(EventID){
        case -1:
            //test error message
            std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
            break;
        case 0:
            initializeTasks();
            break;
        default: 
            //Event ID = destination worker node ID for positive int
            if(EventID>0) doSendTask(EventID);
            else std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
    }
}

//event 0 initialize event
void ParameterServer::initializeTasks(int tasks){
    totalTasks=tasks;
    //for units in worker map
    for (const auto& worker : workers) {
        scheduleSendTask(worker.second->getID());
    }
}

//schedule an arrival for the processing unit
void ParameterServer::scheduleSendTask(int workerID){
    auto env = Environment::getInstance();
    Event task{env->getTime()+thinkTime, nodeID, workerID};
    env->addEvent(task);

    //log event
    std::cout<<"scheduling a send task\n";
}

//gets run when its actually time to send a task to processing unit
//bad hard coded names :(
void ParameterServer::doSendTask(int workerID){
    //log event
    std::cout<<"PS sending task\n";
    workers.at(workerID)->processTask();
}

//Add logic here to control the parameter server's behavior/load balancing
void ParameterServer::processCompletion(ProcessingUnit* sender){
    //log event
    std::cout<<"PS recieved completed task\n";

    ++tasksDone;
    //keep going if necessary
    if(tasksDone<totalTasks){
        scheduleSendTask(sender->getID());
    }
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    //insert pointer to map
    workers[unit->getID()] = unit;
}

}//end namespace