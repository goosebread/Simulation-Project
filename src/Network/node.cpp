#include "node.h"
#include "environment.h"

namespace Network{

Node::Node(){
    //start naming nodes from 0 so they're easy to index in a list
    nodeID = totalNodes;
    ++totalNodes;
    auto env = Environment::getInstance();

    //nodeID are unique and start from zero anyways
    env->nodes.push_back(this);
}

//initial condition
int Node::totalNodes = 0;

}//end namespace