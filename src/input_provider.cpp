#include "input_provider.h"

#include "logger.h"

#include <string>
#include <filesystem>

InputProvider *InputProvider::instance = nullptr;
extern Logger logger;

void InputProvider::Init(std::optional<std::string> file_path){
    if(file_path.has_value() == false) // input from stdin will be read
        InputProvider::SetInstance();
    else
        InputProvider::SetInstance(true, file_path.value());
}

InputProvider* InputProvider::GetInstance(){
    if(!instance){
        SetInstance();
    }
    return instance;
}

void InputProvider::SetInstance(bool fileProvider, std::string filePath){
    if(instance){
        delete instance;
    }
    if(fileProvider){
        instance = new FileInputProvider(filePath);
    }
    else{
        instance = new UserInputProvider();
    }
}

std::string UserInputProvider::GetNextCommand(){
    std::string cmd;
    logger << LogDest(LOG_UCI) << "> ";
    std::getline(std::cin, cmd);
    return cmd;
}

FileInputProvider::FileInputProvider(std::string file_path){
    std::filesystem::path path(file_path);
    input.open(file_path);
    logger << LogDest(LOG_DEBUG) << file_path << "\n";

    if(!input.good()){
        logger << LogDest(LOG_BAD_INPUT) << "Bad input file: quitting";
        exit(1);
    }
}

FileInputProvider::~FileInputProvider(){
    input.close();
    instance = nullptr;
}

std::string FileInputProvider::GetNextCommand(){
    std::string command;
    std::getline(input, command);
    if(!input.good()){
        return std::string("quit"); // quit the program if eof is reached
    }
    logger << LogDest(LOG_DEBUG) << "Input: " << command << "\n";
    return command;
}