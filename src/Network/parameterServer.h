#pragma once
#include <iostream>
#include "node.h"
#include "event.h"
#include "processingUnit.h"

//A temporary class until we figure out the specifics of the parameter server
//right now it just runs 5 hard coded tasks as the initilization function
//and hard-coded to have 1 connection with a processingUnit

//it schedules and runs tasks just fine

namespace Network{

//forward class declaration
class ProcessingUnit;

class ParameterServer : public Node{
friend ProcessingUnit;

private:
    int totalTasks;//when to stop
    int tasksDone;//running total

    int thinkTime=1;//temporary parameter for basic simulation

    ProcessingUnit* worker;//hard-coded connection

    //schedules next task after think time
    void scheduleSendTask();

    //gets called by ProcessingUnit function when task completes
    void processCompletion();

    //event 0 initialize event
    void initializeTasks(int tasks=5){
        totalTasks=tasks;
        scheduleSendTask();
    }

    //event 1, gets called when event handler says time=arrival
    //calls instantaneous functions of other nodes
    void doSendTask();

public:
    ParameterServer():Node(),totalTasks{0}{
    }
    void doEvent(int EventID) override;
    void connectWorker(ProcessingUnit* unit);
    //~ParameterServer(){}
};

}//end namespace