#include "input_provider.h"

#include <string>
#include <filesystem>

InputProvider *InputProvider::instance = nullptr;

InputProvider* InputProvider::GetInstance(){
    if(!instance){
        SetInstance();
    }
    return instance;
}

void InputProvider::SetInstance(bool fileProvider, std::string filePath){
    if(instance){
        std::cerr << "Input Provider already exists" << std::endl;
        return;
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
    std::cout << "> ";
    std::getline(std::cin, cmd);
    return cmd;
}

FileInputProvider::FileInputProvider(std::string file_path){
    std::filesystem::path path(file_path);
    input.open(file_path);
    std::cout << file_path << "\n";
    if(!input.good()){
        std::cout << "Bad input file: quitting";
        exit(0);
    }
}

FileInputProvider::~FileInputProvider(){
    input.close();
}

std::string FileInputProvider::GetNextCommand(){
    std::string command;
    std::getline(input, command);
    if(!input.good()){
        return std::string("quit"); // quit the program if eof is reached
    }
    std::cerr << "Input: " << command << "\n";
    return command;
}