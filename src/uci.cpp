#include "uci.h"
#include "search.h"
#include "functions.h"
#include "evaluate.h"
#include "node.h"
#include "position.h"

#include <thread>
#include <memory>
#include <iostream>

void Uci::loop(){
    bool running = false;
    std::string command;
    std::unique_ptr<std::thread> st = nullptr;
    SearchTree::Init();
    while(true){
        std::cout << "> ";
        std::cin >> command;

        {
            SearchTree* tree = SearchTree::GetInstance();
            if(THREAD_READY_TO_JOIN == tree->GetThreadStatus()){
                if(st->joinable()){
                    st->join();
                    tree->SetThreadStatus(THREAD_IDLE);
                    running = false;
                }
            }
        }

        if(command == "uci")
            std::cout << "uci ok\n";
        if(command == "isready"){
            std::cout << "readyok\n";
        }
        if(command == "position"){
            std::string option;
            std::cin >> option;
            if(option == "fen"){
                std::string fen;
                std::cin >> fen;
                SearchTree::Init(fen);
            }
            if(option == "startpos"){
                //not implemanted
            }
        }

        if(command == "go"){
            if(running == false){
                SearchTree::Init();
                SearchTree *tree = SearchTree::GetInstance();
                int depth;
                std::cin >> depth;
                if(depth == 0){
                    std::cout << "depth must be positive\n";
                    continue;
                }
                running = true;
                tree->SetThreadStatus(THREAD_RUNNING);
                st = std::make_unique<std::thread>(std::thread(executeSearching, depth));
            }
        }

        if(command == "ucinewgame"){
            SearchTree::Init();
        }

        if(command == "stop"){
            if(running){
                SearchTree* tree = SearchTree::GetInstance();
                tree->SetThreadStatus(THREAD_STOP);
                st->join();
                tree->SetThreadStatus(THREAD_IDLE);
                running = false;
            }

        }

        if(command == "ponderhit"){
            // nothing yet
        }
        if(command == "quit"){
            if(running){
                SearchTree* tree = SearchTree::GetInstance();
                tree->SetThreadStatus(THREAD_STOP);
                st->join();
                tree->SetThreadStatus(THREAD_IDLE);
                running = false;
            }
            break;
        }


        // not uci

        if(command == "board"){
            SearchTree *tree = SearchTree::GetInstance();
            tree->ShowBoard();
        }
        if(command == "eval"){
            SearchTree *tree = SearchTree::GetInstance();
            tree->GetEntryNode()->Evaluate();
        }
        if(command == "hash"){
            SearchTree *tree = SearchTree::GetInstance();
            std::cout << tree->GetEntryNode()->position->GetPositionHash();
        }
        if(command == "sethash"){
            std::cin >> Evaluator::hashInfo;
        }
    }
    //if(searching_thread)
    //    delete searching_thread;
    return;
}