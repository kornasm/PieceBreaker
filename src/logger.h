#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <boost/format.hpp>

class Node;

// logging levels for logger (bit mask)
// 0 - be quiet, log only those messages that have LOG_ALWAYS level
// 1 - uci level (every data required by uci protocol should be logged with that level)
// 2 - additional data about analasis done by engine
// 4 - additional info about internal process state

#define LOG_QUIET 0
#define LOG_UCI 1
#define LOG_ANALYSIS 2
#define LOG_DEBUG 4
#define LOG_ALWAYS 7 // change the value when adding new log levels

// a few more defines just for code readability
#define LOG_ERROR LOG_ALWAYS
#define LOG_BAD_INPUT LOG_ALWAYS

class LogDest{
    public:
        int level;
        LogDest(int lev) :level(lev) {};
};

class Logger{
    private:
        std::ostream *out = &std::cout;
        std::ofstream of;

        int logging_level = LOG_UCI;
        int msg_level = LOG_UCI;
        void Print(std::string msg);
        bool ShouldLog(int level);
    
    public:
        Logger(int level = 1);

        int GetLevel() { return logging_level ;}
        void SetLevel(int l) { logging_level = l; }

        // even though this implementation covers most case, some additional function are needed
        template<typename T>
        Logger& operator<<(T object){
            if(ShouldLog(msg_level)){
                *out << object;
            }
            return *this;
        }
        Logger& operator<<(Node& nd);

        // logging with streams is preferred
        // Use functions below only when it's impossible to use streams
        void Log(std::string msg, int level = 1);
        // std::format will be used as soon as it will get support in g++
        void Log(boost::format fmt, int level = 1);

        void LogEvaluation(Node *node, int level = LOG_ANALYSIS);
};

template<>
Logger& Logger::operator<<(LogDest ld);

#endif
