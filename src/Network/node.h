#pragma once

#include <armadillo>

namespace Network{

class Node{
    private:
        double data1;
        arma::vec v;
    public:
        Node(double data=0);
        double getData1() const;
};

}//end namespace