#include "logger.h"

namespace Utils{

//initialize static variable
Logger* Logger::instance = nullptr;

Logger::Logger(){
    std::ifstream ifile;
    ifile.open(filepath_base+".txt");
    if(ifile){
        //find a name that doesn't exist yet
        int num=1;
        //while file is not a null pointer
        while(ifile){
            ifile.close();
            filepath = filepath_base + std::to_string(num);
            ifile.open(filepath+".txt");
            ++num;
        }
    }
    else{
        filepath = filepath_base;
    }
    //we know this file  doesn't exist yet since it can't open for reading
    //so we can use it for writing
    file.open(filepath+".txt");
}

Logger::~Logger(){
    file<<std::flush;
    file.close();
}

}//end namespace Utils