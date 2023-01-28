#include "logger.h"

#include <filesystem>
#include <iostream>

Logger logger = Logger();

Logger::Logger( int level){
    logging_level = level;
}

void Logger::Print(std::string msg){
    std::cout << msg << '\n';
}

void Logger::Log(std::string msg, int level){
    if(logging_level && level){
        Print(msg);
    }
}

void Logger::LogEvaluation(Node *node){
    Print(std::to_string(node->partialEval));
}