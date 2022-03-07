#pragma once

#include "node.h"

namespace Network{

//add functions later to be able to record traffic statistics
class link{
    private:
        Node* end1;
        Node* end2;
    public:
        double getData1() const;
};

}//end namespace