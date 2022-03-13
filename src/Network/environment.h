#pragma once
#include <queue>
#include <vector>
#include <functional>
#include <iostream>

#include "event.h"
#include "node.h"

//The environment/event handler keeps track of a global time during the simulation 
//and calls scheduled events in the order that they're scheduled to occur
//This is the class that controls the main simulation

namespace Network{

class Environment{
private:
    //enforce singleton
    //there can only be one Environment object existing
    static Environment* instance;
    Environment();

    //queue of events, ordered by time of occurrance
    std::priority_queue<Network::Event, std::vector<Network::Event>, std::less<Network::Event> > events;

    //"global" variable accessible and updated during simulation
    double currentTime;

public:
    //enforce singleton
    //call the static method getInstance() 
    //instead of trying to create Environment objects yourself
    static Environment* getInstance() {
        if (!instance)
        instance = new Environment();
        return instance;
    }

    //list of all nodes
    //the unique integer node IDs are used as the index for each node object
    std::vector<Node*> nodes;

    //wrapper for adding to priority queue (public access)
    void addEvent(Event e){ events.push(e); }

    //get current time during simulation
    double getTime()const{ return currentTime; }

    //make sure to add initial task to queue
    //before running the simulation
    void runSim();
};

}//end namespace network