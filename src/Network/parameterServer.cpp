#include "parameterServer.h"

namespace Network{

//legal functions for event handler to call
void ParameterServer::doEvent(int EventID){
    switch(EventID){
        case 0:
            initializeTasks();
            break;
        case 1: 
            doSendTask();
            break;
        default:
            std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
    }
}

//schedule an arrival for the processing unit
void ParameterServer::scheduleSendTask(){
    auto env = Environment::getInstance();
    Event task{env->getTime()+thinkTime, nodeID,1};
    env->addEvent(task);

    //log event
    std::cout<<"scheduling a send task\n";
}

//gets run when its actually time to send a task to processing unit
//bad hard coded names :(
void ParameterServer::doSendTask(){
    //log event
    std::cout<<"PS sending task\n";

    worker->processTask();
}

//Add logic here to control the parameter server's behavior/load balancing
void ParameterServer::processCompletion(){
    //log event
    std::cout<<"PS recieved completed task\n";

    ++tasksDone;
    //keep going if necessary
    if(tasksDone<totalTasks){
        scheduleSendTask();
    }
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    worker = unit;
}

}//end namespace