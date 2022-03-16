#pragma once
#include <iostream>
#include <random>

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

    //random components
    std::mt19937 rng;
    std::normal_distribution<double> gaussian;

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
    ProcessingUnit(double service_mean, double service_stdev, unsigned seed)
        :Node(),rng{seed},gaussian{service_mean,service_stdev}{
    }

    void connectController(ParameterServer* unit);
    void doEvent(int EventID) override;
    //~ProcessingUnit(){}
};

}//end namespace