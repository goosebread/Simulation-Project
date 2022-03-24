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
void ParameterServer::initializeTasks(){
    tasksDone=0;
    for (auto& worker : workers) {
        schedulePull(worker.second->getID());
        worker.second->reset();
    }
}

//some network traffic delay between pushing and pulling
void ParameterServer::schedulePull(int workerID){
    auto env = Environment::getInstance();

    constexpr double networkDelay = 0.5;//TODO MAKE THIS REAL VARIABLE

    Event task{env->getTime()+networkDelay, nodeID, workerID};
    env->addEvent(task);

    //log event
    Utils::Logger::getInstance()->file<<"scheduling pull\n";
}

//gets run when its actually time to send a task to processing unit
//bad hard coded names :(
//"departure"
void ParameterServer::doPull(int workerID){
    workers.at(workerID)->processPull();
}

void ParameterServer::processPush(int workerID,int iteration){
    //log event
    Utils::Logger::getInstance()->file<<"PS recieved push\n";

    //will start at index 1 to indicate that task 1 for iteration 1 is complete
    updateTracker.at(iteration%windowSize)->at(workerID) = true;
    
    //exit condition for simulation //NOTE if a worker reaches the last iteration, 
    //it will be idle but not in idleWorkers, which is meant as a temporary holding space
    if(iteration<totalTasks){
        //send to holding list if too far ahead
        //otherwise schedule next
        if (iteration==tasksDone+1+windowSize){
            Utils::Logger::getInstance()->file<<"worker "<<workerID<<" to idle\n";
            idleWorkers.push_front(workerID);
        }
        else{
            //if this iteration is the bottleneck for PS
            if(iteration==tasksDone+1){
                checkUpdate();
            }
            schedulePull(workerID);
        }
    }
    else{
        Utils::Logger::getInstance()->file<<"worker "<<workerID<<" reached last iteration\n";
    }
}

void ParameterServer::checkUpdate(){
    //update if all workers are done/TRUE for the next task
    bool status = true;
    auto mapRef = *updateTracker.at((tasksDone+1)%windowSize);
    for (const auto& workerStatus : mapRef) {
        status = status && workerStatus.second;
        if (!status) break;
    }
    if(status){
        tasksDone++;
        //release workers in holding
        while (!idleWorkers.empty()){
            schedulePull(idleWorkers.front());
            idleWorkers.pop_front();
        }
                
        //reset tracker
        for (auto& workerStatus : mapRef) {
            workerStatus.second = false;
        }
        Utils::Logger::getInstance()->file<<"PS iteration "<<tasksDone<<" complete\n";
    }
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    //insert pointer to map
    workers[unit->getID()] = unit;
    for (int i=0; i<windowSize; i++){
        updateTracker[i]->operator[](unit->getID())=false;
    }
}

}//end namespace