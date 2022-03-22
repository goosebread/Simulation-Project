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
            if(EventID>0) doPull(EventID);
            else std::cerr<<"INVALID EVENT ID "<<EventID<<" FROM NODE "<<getID();
    }
}

//event 0 initialize event
void ParameterServer::initializeTasks(int tasks){
    busy=false;
    totalTasks=tasks;
    tasksDone=0;
    schedulePull();
    
}

//schedule an arrival for the processing unit
//this process has think time
void ParameterServer::schedulePull(){
    if(tasksDone<totalTasks){
        auto env = Environment::getInstance();
        double thinkTime = gaussian(rng);
        for (const auto& worker : workers) {
            Event task{env->getTime()+thinkTime, nodeID, worker.second->getID()};
            env->addEvent(task);
        }
        //log event
        Utils::Logger::getInstance()->file<<"scheduling pull\n";
    }
    else{
        Utils::Logger::getInstance()->file<<"All tasks done/max iterations reached\n";
    }
}

//gets run when its actually time to send a task to processing unit
//bad hard coded names :(
//"departure"
void ParameterServer::doPull(int workerID){
    workers.at(workerID)->processPull();
}

void ParameterServer::processPush(int workerID){
    //log event
    Utils::Logger::getInstance()->file<<"PS recieved push\n";

    updateTracker.at(workerID) = true;
    checkUpdate();
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    //insert pointer to map
    workers[unit->getID()] = unit;
    updateTracker[unit->getID()]=false;
}

void ParameterServer::checkUpdate(){
    bool status = true;
    for (const auto& workerStatus : updateTracker) {
        status = status && workerStatus.second;
        if (!status) break;
    }
    if(status){
        tasksDone++;
        schedulePull();
        //reset tracker
        for (auto& workerStatus : updateTracker) {
            workerStatus.second = false;
        }
    }
}

}//end namespace