#ifndef UCI_H_
#define UCI_H_

#include <memory>
#include <thread>
#include <sstream>

class Node;
class SearchTree;
class InputProvider;

class Uci{
    private:
        std::string command, option;
        std::unique_ptr<std::thread> st = nullptr;
        std::unique_ptr<std::thread> side_thr = nullptr;
        std::unique_ptr<SearchTree> tree;
        InputProvider *ip;
        bool running = false;
        std::stringstream sstr;
        bool loop = true;


    public:
        Uci();
        void Loop();

    private:

        // UCI
            
            void UciConfirm();
            void IsReady();
            void Position();
            void Go();
            void UciNewGame();
            void Stop();
            void Quit();

        // NOT UCI (development + debugging)
            
            // visual overview of search tree
            // it gives much more data than debuggers
            void ExploreRoot();

            void ShowBoard();
            void Eval();
            
            // hash for printing more evaluation data about chosen position
            void GetHash();
            void SetHash();

        // TESTS
            void Sleep();


        // Not a commands
        
            // for cases when there threads can be joined (nothing more to search)
            // but engine didn't get "stop" command
            void JoinThreads();
};


#endif