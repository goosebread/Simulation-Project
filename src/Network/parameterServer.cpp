#include "parameterServer.h"
#include "../Utils/logger.h"

namespace Network{

ParameterServer::ParameterServer(int tasks, int windowSz)
    :Node(),totalTasks{tasks},windowSize{windowSz}{

    //window = 0 case
    updateTracker.push_back(new std::map<int,bool>());
    //windowsize > 0
    for (int i=1; i<windowSize+1; i++){
        updateTracker.push_back(new std::map<int,bool>());
    }
}

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

    constexpr double networkDelay = 0.01;//TODO MAKE THIS REAL VARIABLE

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
    int index = 0;
    if(windowSize){
        index = iteration%(windowSize+1);
    }

    updateTracker[index]->operator[](workerID) = true;

    //exit condition for simulation //NOTE if a worker reaches the last iteration, 
    //it will be idle but not in idleWorkers, which is meant as a temporary holding space
    if(iteration<totalTasks){
        //send to holding list if too far ahead
        //otherwise schedule next
        if (iteration==tasksDone+1+windowSize){
            Utils::Logger::getInstance()->file<<"worker "<<workerID<<" to idle\n";
            idleWorkers.push_front(workerID);

            //zero window case
            if(!windowSize){
                checkUpdate();
            }
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

    int index = 0;
    if(windowSize){
        index = (tasksDone+1)%(windowSize+1);
    }
    auto mapRef = *updateTracker.at(index);

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

            //FIX LATER i think the reference may not be set up properly which is why this doesn't like to reset
            updateTracker[index]->operator[](workerStatus.first)=false;
        }
        Utils::Logger::getInstance()->file<<"PS iteration "<<tasksDone<<" complete\n";
    }
}

//used to define the network
void ParameterServer::connectWorker(ProcessingUnit* unit){
    //insert pointer to map
    workers[unit->getID()] = unit;
    updateTracker[0]->operator[](unit->getID())=false;

    //for window>0
    for (int i=1; i<windowSize+1; i++){
        updateTracker[i]->operator[](unit->getID())=false;
    }
}

Stats ParameterServer::outputStats(){
    Stats s = Stats();
    double time = Environment::getInstance()->getTime();
    //WARNING the last task doesn't actually get marked as done; the simulation just stops. hence the +1
    s.throughput = (tasksDone+1)/time;
    double total_busy = 0;
    for (const auto worker:workers){
        total_busy+=worker.second->t_busy;
        std::cout<<worker.second->t_busy<<std::endl;
    }
    std::cout<<time<<std::endl;
    std::cout<<workers.size()<<std::endl;
    s.avgUtilization = total_busy/(time*workers.size());
    return s;
}

}//end namespace