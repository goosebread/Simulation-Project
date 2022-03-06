#pragma once

#include <armadillo>

namespace Network{

class Node{
    private:
        double data1;
        arma::vec v;
    public:
        Node(double data);
        double getData1() const;
};

}//end namespace