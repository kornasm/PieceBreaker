#include "uci.h"

#include "search.h"
#include "functions.h"
#include "evaluate.h"
#include "node.h"
#include "position.h"

#include <thread>
#include <iostream>

void Uci::loop(){
    bool running = false;
    std::string command;
    std::unique_ptr<std::thread> st = nullptr;
    SearchTree *tree = SearchTree::GetInstance();
    while(true){
        std::cin >> command;        

        if(running){
            if(THREAD_READY_TO_JOIN == tree->GetThreadStatus()){
                if(st->joinable()){
                    st->join();
                    tree->SetThreadStatus(THREAD_IDLE);
                    running = false;
                    SearchTree::Init(SearchTree::fen);
                    tree = SearchTree::GetInstance();
                }
            }
        }

        if(command == "uci")
            std::cout << "uciok" << std::endl;
        if(command == "isready")
            std::cout << "readyok" << std::endl;

        if(command == "position"){
            std::string option;
            std::cin >> option;
            if(option == "fen"){
                std::getline(std::cin, SearchTree::fen);
                SearchTree::Init(SearchTree::fen);
            }
            if(option == "startpos"){
                //not implemanted
            }
        }

        if(command == "go"){
            if(running == false){
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
                tree->SetThreadStatus(THREAD_STOP);
                st->join();
            }
            break;
        }


        // not uci

        if(command == "board"){
            tree->ShowBoard();
        }
        if(command == "eval"){
            tree->GetEntryNode()->Evaluate();
        }
        if(command == "hash"){
            std::cout << tree->GetEntryNode()->GetHash() << '\n';
        }
        if(command == "sethash"){
            std::cin >> Evaluator::hashInfo;
        }
    }
    return;
}