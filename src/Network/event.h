#pragma once
//An event is defined by a time of occurrance 
//and a Node/Event identifier for the appropriate function that will occur

namespace Network{

struct Event{
    double time;
    int NodeID;
    int EventID;
    Event(double t, int nID, int eID):time{t},NodeID{nID},EventID{eID}{
    }

    //define comparison operators for sorting based on time/min heap priority queue structure
    //environment.h's priority queue uses std::greater which will use the overloaded operator
    bool operator>(const Event& other) const{
        return other.time>time;
    }
    bool operator>=(const Event& other) const{
        return other.time>=time;
    }
    bool operator<(const Event& other) const{
        return other.time<time;
    }
    bool operator<=(const Event& other) const{
        return other.time<=time;
    }
};

}//end namespace