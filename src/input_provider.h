#ifndef INPUT_PROVIDER_H_
#define INPUT_PROVIDER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <optional>

class InputProvider{
    public:
        static void Init(std::optional<std::string> file_path);
        static InputProvider* instance;
        static InputProvider* GetInstance();
        static void SetInstance(bool fileProvider = false, std::string filePath = "");
        virtual std::string GetNextCommand() = 0;
        virtual ~InputProvider(){};
    protected:
        InputProvider(){};

};

class UserInputProvider: InputProvider{
    public:
        std::string GetNextCommand() override;
        ~UserInputProvider() { instance = nullptr; };
    private:
        friend InputProvider;
        UserInputProvider(){};
};

class FileInputProvider: InputProvider{
    public:
        std::string GetNextCommand() override;
        ~FileInputProvider();

    private:
        
        FileInputProvider(std::string file_path);
        friend InputProvider;
        std::ifstream input;
};

#endif
