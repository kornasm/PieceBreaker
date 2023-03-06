#ifndef INPUT_PROVIDER_H_
#define INPUT_PROVIDER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <optional>

class InputProvider{
    protected:
        static InputProvider* instance;

    protected:
        InputProvider(){};

    public:

        virtual ~InputProvider(){};

        static void Init(std::optional<std::string> file_path);
        static InputProvider* GetInstance();
        static void SetInstance(bool fileProvider = false, std::string filePath = "");
        virtual std::string GetNextCommand() = 0;

};

class UserInputProvider: InputProvider{
    private:
        UserInputProvider(){};

    public:
        ~UserInputProvider() { instance = nullptr; }

        std::string GetNextCommand() override;

        friend InputProvider;
};

class FileInputProvider: InputProvider{
    private:
        std::ifstream input;

        FileInputProvider(std::string file_path);

    public:
        ~FileInputProvider();

        std::string GetNextCommand() override;

        friend InputProvider;
};

#endif
