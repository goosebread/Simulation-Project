#pragma once
#include <iostream>
#include <map>
#include <random>

#include "node.h"
#include "event.h"
#include "processingUnit.h"

//A temporary class until we figure out the specifics of the parameter server
//right now it just runs 5 hard coded tasks as the initilization function
//and hard-coded to have 1 connection with a processingUnit

//assuming homogenous tasks, round-robin initialization, tasks are sent to whoever's idle after that for load balancing
//(task 1 and task 2 are interchangable: all that matters is 5 tasks get run) 
//program stops when the last task is received by PS. workers may still be doing extra tasks

//it schedules and runs tasks just fine

namespace Network{

//forward class declaration
class ProcessingUnit;

class ParameterServer : public Node{
friend ProcessingUnit;

private:
    int totalTasks;//when to stop
    int tasksDone;//running total

    //random components
    std::mt19937 rng;
    std::normal_distribution<double> gaussian;

    std::map<int,ProcessingUnit*> workers;//hard-coded connections

    //schedules next task after think time
    void scheduleSendTask(int workerID);

    //gets called by ProcessingUnit function when task completes
    //send the next task to this sender
    void processCompletion(ProcessingUnit* sender);

    //event 0 initialize event
    void initializeTasks(int tasks=5);

    //event 1, gets called when event handler says time=arrival
    //calls instantaneous functions of other nodes
    void doSendTask(int workerID);

    //sends a task to the queue
    void toQueue(int workerID);
public:
    bool busy;

    //queue of destinations to send tasks to (think time process is limited by one processor)
    std::queue<int> sendQueue;

    ParameterServer(double think_mean, double think_stdev, unsigned seed)
        :Node(),rng{seed},gaussian{think_mean,think_stdev},totalTasks{0}{
    }
    void doEvent(int EventID) override;
    void connectWorker(ProcessingUnit* unit);
    //~ParameterServer(){}
};

}//end namespace