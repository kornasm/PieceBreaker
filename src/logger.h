#ifndef LOGGER_H_
#define LOGGER_H_

#include "node.h"
#include "search.h"

#include <string>
#include <fstream>

// logging levels (bit mask)
// 0 - when standard mesgs done by Logger::Log (used for tests, errors)
// 1 - uci level (every data required by uci protocol should be logged with that level)
// 2 - additional data about analasis done by engine
// 4 - additional info about internal process state

#define LOG_QUIET 0
#define LOG_UCI 1
#define LOG_ANALYSIS 2
#define LOG_DEBUG 4

class Logger{
    private:
        //static Logger *instance;
        std::ofstream output;
        int logging_level;
        void Print(std::string msg);
    
    public:
        Logger(int level = 1);
        void LogEvaluation(Node *node);
        void Log(std::string msg, int level = 1);

        int GetLevel() { return logging_level ;}
        int SetLevel(int l) { logging_level = l; }
};

#endif