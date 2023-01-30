#include "logger.h"

#include "search.h"
#include "node.h"
#include "move.h"

#include <iostream>

Logger logger = Logger();

Logger::Logger( int level){
    logging_level = level;
}

void Logger::Print(std::string msg){
    std::cout << msg << '\n';
}

bool Logger::ShouldLog(int level){
    if(level == LOG_ALWAYS){
        return true;
    }
    else{
        return (bool)(level & this->logging_level);
    }
}

void Logger::Log(std::string msg, int level){
    if(ShouldLog(level)){
        Print(msg);
    }
}

void Logger::Log(boost::format fmt, int level){
    if(ShouldLog(level)){
        Print(fmt.str());
    }
}

void Logger::LogEvaluation(Node *node, int level){
    if(ShouldLog(level)){
        Print(std::to_string(node->partialEval));
    }
}

Logger& Logger::operator<<(LogDest ld){
    this->msg_level = ld.level;
    return *this;
}