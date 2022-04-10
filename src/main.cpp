#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>

#include "Network/environment.h"
#include "Network/parameterServer.h"
#include "Network/processingUnit.h"
#include "Network/event.h"

Network::Stats createSim(int iters, int n_Workers, int window, double CV, unsigned seed, bool verbose){
    //should be good enough for generating seeds
    std::srand(seed);

    //create controller and worker
    Network::ParameterServer* controller = new Network::ParameterServer(iters, window);//hard coded network delay 0.01

    std::vector<Network::ProcessingUnit*> workers;
    for (int i=0; i<n_Workers;i++){
        auto wkr = new Network::ProcessingUnit(CV,std::rand());
        workers.push_back(wkr);

        //link
        controller->connectWorker(wkr);
        wkr->connectController(controller);
    }

    //add initialization event
    Network::Environment* env = Network::Environment::getInstance();
    env->addEvent(Network::Event(0,0,0));

    //run
    env->runSim();
    
    Network::Stats s = controller->outputStats();

    if (verbose){
        std::cout<<"Simulation Complete\n";
        std::cout<<"Throughput = "<<s.throughput<<std::endl;
        std::cout<<"Average Utilization = "<<s.avgUtilization<<std::endl;
    }

    //clean up
    delete controller;
    for (int i=0; i<n_Workers;i++){
        delete workers[i];
    }
    env->reset();

    return s;
}


int main(){
    std::string filepath = "SimInputs.csv";

    std::ifstream fin;
    fin.open(filepath);
    if(!fin){
        std::cerr<<"SimInputs.csv File not Found";
        return 0;
    }

    std::ofstream fout;
    fout.open("SimOutputs.csv");
  
    //ignore header row
    std::string line;
    std::getline(fin, line);
    fout<<line<<",Throughput,AvgUtilization"<<std::endl;

    //data rows
    while (std::getline(fin, line)){
        // used for breaking words
        std::stringstream s(line);
  
        // read every column data of a row and
        // store it in a string variable, 'word'
        std::vector<std::string> row{};
        std::string word;

        while (std::getline(s, word, ',')) {
            row.push_back(word);
        }
  
        if (row.size()!=5){
            std::cerr<<"Wrong number of arguments";
            return 0;
        }

        int iters = std::stoi(row[0]);//10000;
        int n_Workers = std::stoi(row[1]);//3;
        int window = std::stoi(row[2]);//12;
        double CV = std::stod(row[3]);//2
        uint seed = std::stoi(row[4]);

        Network::Stats stats = createSim(iters,n_Workers,window,CV,seed,false);
        fout<<line<<","<<stats.throughput<<","<<stats.avgUtilization<<std::endl;
    }
    fin.close();
    fout<<std::flush;
    fout.close();
    return 0;
}