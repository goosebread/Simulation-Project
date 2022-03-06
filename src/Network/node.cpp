#include "node.h"

namespace Network{

Node::Node(double data){
    data1=data;
}

double Node::getData1() const{
    return data1;
}

}//end namespace