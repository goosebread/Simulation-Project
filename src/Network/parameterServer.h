#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <forward_list>

#include <random>

#include "node.h"
#include "event.h"
#include "processingUnit.h"

//vector of units, "hold/wait"
//send units to the wait vector if they're too far ahead
//upon update empty the hold vector and let them pull

//create a matrix as the rotating window (mod windowsize to reuse space as things fill up)
//each row is an iteration group
//each col is worker status (0 for not done yet, 1 for done)

//or list/vector of "iterationStatus structs/maps"

//we need more information being communicated between ps and workers (eg iteration group )


namespace Network{

//forward class declaration
class ProcessingUnit;

class ParameterServer : public Node{
friend ProcessingUnit;

private:
    int totalTasks;//when to stop/max iterations
    int tasksDone;//current iteration

    int windowSize;//for bounded delay model

    //random components
    std::mt19937 rng;
    std::normal_distribution<double> gaussian;

    //keeps track of workers to call their functions
    std::map<int,ProcessingUnit*> workers;

    //vector could be replaces with fixed size array (window size fixed)
    //map could be simplified to array if worker IDs are guaranteed to be integers (safer to go more general key pair route)
    //each index t of vector is a slot in the time window
    //each value in vector is a list of workers completion status for iteration t
    std::vector<std::map<int,bool>*> updateTracker;

    //place to store workers that are too far ahead and must wait for PS to update
    std::forward_list<int> idleWorkers;//holding

    //schedules a pull event
    void schedulePull(int workerID);

    //event 1, calls instantaneous functions of other nodes
    void doPull(int workerID);

    //gets called by ProcessingUnit function when task completes
    //calls checkUpdate
    void processPush(int workerID, int iteration);

    //checks to see if the Parameter server can move on to the next iteration
    void checkUpdate();

    //event 0 initialization event
    void initializeTasks();

public:
    ParameterServer(double think_mean, double think_stdev,int tasks, int windowSz, unsigned seed)
        :Node(),rng{seed},gaussian{think_mean,think_stdev},totalTasks{tasks},windowSize{windowSz}{
        for (int i=0; i<windowSize; i++){
            updateTracker.push_back(new std::map<int,bool>());
        }
    }
    void doEvent(int EventID) override;
    void connectWorker(ProcessingUnit* unit);

    ~ParameterServer(){
        for (int i=0; i<windowSize; i++){
            delete updateTracker[i];
        }
    }
};

}//end namespace