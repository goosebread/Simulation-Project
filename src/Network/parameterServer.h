#pragma once
#include <iostream>
#include <map>
#include <random>

#include "node.h"
#include "event.h"
#include "processingUnit.h"

//gen 1 parameter server
//the PS has a max iterations /total tasks value
//it needs all workers to finish pushing on iteration i before starting iteration i+1
//think time/update time is built in (time between the last push of iteration i and the pulls of iteration i+1)
//assume that all workers can pull at same time

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
    std::map<int,bool> updateTracker;//could turn this into regular array with normal indexing in future
    //if we need constant access time as opposed to log n access

    //schedules next task after think/update time
    //all workers pull at the same time/synchronized
    void schedulePull();

    //gets called by ProcessingUnit function when task completes
    //send the next task to this sender
    //check update
    void processPush(int workerID);

    //event 0 initialize event
    void initializeTasks(int tasks=3);

    //event 1, gets called when event handler says time=arrival
    //calls instantaneous functions of other nodes
    void doPull(int workerID);

    void checkUpdate();
    //sends a task to the queue
    //void toQueue(int workerID);
public:
    bool busy;

    //queue of destinations to send tasks to (think time process is limited by one processor)
    //std::queue<int> sendQueue;

    ParameterServer(double think_mean, double think_stdev, unsigned seed)
        :Node(),rng{seed},gaussian{think_mean,think_stdev},totalTasks{0}{
    }
    void doEvent(int EventID) override;
    void connectWorker(ProcessingUnit* unit);
    //~ParameterServer(){}
};

}//end namespace