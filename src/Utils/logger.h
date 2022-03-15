#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace Utils{

class Logger{
private:
    const std::string filepath_base = "SimLog";
    std::string filepath;

    //enforce singleton
    static Logger* instance;
    Logger();
    ~Logger();

public:
    //just use this?
    std::ofstream file;

    static Logger* getInstance() {
        if (!instance)
        instance = new Logger();
        return instance;
    }
    void operator()(const std::string &s){
        file<<s;
    }
    void operator()(int i){
        file<<i;
    }
    void operator()(double d){
        file<<d;
    }
};

}//end namespace Utils