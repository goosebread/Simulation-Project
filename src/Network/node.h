#pragma once

//deletion of nodes is not supported
//nodes are given ID values based on order of creation
//The constructor can only be accessed through child classes; node is an abstract class

namespace Network{

class Node{
protected:
    int nodeID;
    Node();
public:
    static int totalNodes;//could move to protected if only the constructors use this
    int getID() const{ return nodeID; }

    //calls a function depending on eventID code
    virtual void doEvent(int EventID)=0;
    //virtual ~Node()=0;
};

}//end namespace

//TODO it might be more readable if functions were named schedule_<name> and event_<name>
//if they schedule events or execute/do them