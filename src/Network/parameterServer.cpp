#include "parameterServer.h"
#include "../Utils/logger.h"

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
    busy=false;
    totalTasks=tasks;
    tasksDone=0;
    //for units in worker map
    for (const auto& worker : workers) {
        toQueue(worker.second->getID());
    }
}

//"arrival"
void ParameterServer::toQueue(int workerID){
    if(busy){
        sendQueue.push(workerID);
    }
    else{
        scheduleSendTask(workerID);
        busy=true;
    }
}

//schedule an arrival for the processing unit
//this process has think time
void ParameterServer::scheduleSendTask(int workerID){
    auto env = Environment::getInstance();
    double thinkTime = gaussian(rng);
    Event task{env->getTime()+thinkTime, nodeID, workerID};
    env->addEvent(task);

    //log event
    Utils::Logger::getInstance()->file<<"scheduling a send task\n";
}

//gets run when its actually time to send a task to processing unit
//bad hard coded names :(
//"departure"
void ParameterServer::doSendTask(int workerID){

    //log event
    Utils::Logger::getInstance()->file<<"PS sending task\n";
    workers.at(workerID)->processTask();

    //start next process
    if (sendQueue.empty()){
        busy=false;
    }
    else{
        scheduleSendTask(sendQueue.front());
        sendQueue.pop();
    }
}

//Add logic here to control the parameter server's behavior/load balancing
void ParameterServer::processCompletion(ProcessingUnit* sender){
    //log event
    auto logger = Utils::Logger::getInstance();
    logger->file<<"PS recieved completed task\n";

    ++tasksDone;
    //keep going if necessary
    if(tasksDone<totalTasks){
        toQueue(sender->getID());
    }
    else{
        logger->file<<"No new tasks; PS is done\n"; 
    }
    //logger->file<<"total tasks "<<totalTasks<<" tasks Done "<<tasksDone<<std::endl;
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    //insert pointer to map
    workers[unit->getID()] = unit;
}

}//end namespace