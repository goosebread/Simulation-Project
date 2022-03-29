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
    //its honestly easier to expose this to the caller
    //but we just have to be dilligent about not abusing this
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