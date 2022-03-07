#pragma once
#include "node.h"
namespace Network{

class ProcessingUnit : public Node{
    private:
        double failureRate;
        double processingSpeed;
    public:
        ProcessingUnit(double rate, double speed);
};

}//end namespace