#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <forward_list>

#include <random>

#include "node.h"
#include "event.h"
#include "processingUnit.h"

//Single Parameter Server to allow workers to do pull requests and push updates
namespace Network{

//Stats that are tracked and returned as simulation output
struct Stats{
    double throughput;//throughput calculated using adjected Execution time
    double avgUtilization;//utilization averaged across all worker units
    double adjustedExecutionTime;//1 time unit is the average iteration step time for a 1 worker system
                                 //The ideal linear PS model is assumed where the time it takes n workers to do 1 iteration step is 1/n time units
    std::vector<double> idleHistogram; //histogram starting from 0 for how often n workers are idle
    Stats(double tpt=0, double utl=0, double ext=0):throughput{tpt}, avgUtilization{utl}, adjustedExecutionTime{ext}{}
};

//forward class declaration
class ProcessingUnit;

class ParameterServer : public Node{
friend ProcessingUnit;

private:
    int totalTasks;//when to stop/max iterations
    int tasksDone;//current iteration

    int windowSize;//for bounded delay model

    //negligable network delay and weight update time
    //the simulation works better when the next event doesn't trigger instantaneously
    static constexpr double networkDelay = 0.01;

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

    //records histogram of worker idle frequency
    std::vector<double> idleHistogram;
    int maxIdle;
    double t_last;
    void updateHistogram();

public:
    ParameterServer(int tasks, int windowSz);
    void doEvent(int EventID) override;
    void connectWorker(ProcessingUnit* unit);

    ~ParameterServer(){
        for (int i=0; i<windowSize; i++){
            delete updateTracker[i];
        }
    }

    //If we ever need it:
    //void updateStats() override;

    Stats outputStats();
};

}//end namespace