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
    int iteration;
    
    bool busy;
    double t_last;
    bool busy_last;
    double t_busy;

    //random components
    //actually, a gamma distribution is appropriate
    //we want positive values, a mean of 1, and some control over variance
    std::mt19937 rng;
    std::gamma_distribution<double> gamma;

    //hard coded connection
    ParameterServer* controller;

    //sends time of completion to event handler
    void schedulePush();

    //gets called when task arrives/send event occurs
    void processPull();

    //event 1, gets called when event handler says time=completion
    //calls instantaneous functions of other nodes
    void doPush();

    void reset();

public:
    ProcessingUnit(double CV, unsigned seed)
        :Node(),rng{seed},gamma{1.0/(CV*CV),(CV*CV)},
        iteration{0},t_last{0},t_busy{0},busy{false},busy_last{false}{
    }

    void connectController(ParameterServer* unit);
    void doEvent(int EventID) override;

    //for now it only tracks utilization. Eventually, we might want higher order stats
    void updateStats() override;
    //~ProcessingUnit(){}
};

}//end namespace