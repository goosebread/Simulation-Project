#pragma once
#include <iostream>
#include "node.h"
#include "environment.h"
#include "parameterServer.h"

//A temporary class until we figure out the specifics of the worker units
//right now it just runs tasks when recieved with a hard-coded service time
//and its hard-coded to have 1 connection with a parameterServer

//it schedules and runs tasks just fine

namespace Network{

//forward class declaration
class ParameterServer;

class ProcessingUnit : public Node{
friend ParameterServer;

private:
    //hard coded service time
    double serviceTime=2;

    //hard coded connection
    ParameterServer* controller;

    //sends time of completion to event handler
    void scheduleCompletion();

    //gets called when task arrives/send event occurs
    void processTask(){
        scheduleCompletion();
    }

    //event 1, gets called when event handler says time=completion
    //calls instantaneous functions of other nodes
    void doCompletion();

public:
    ProcessingUnit():Node(){
    }
    void connectController(ParameterServer* unit);
    void doEvent(int EventID) override;
    //~ProcessingUnit(){}
};

}//end namespace