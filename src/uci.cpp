#include "uci.h"

#include "search.h"
#include "functions.h"
#include "evaluate.h"
#include "position.h"
#include "move.h"

#include <thread>
#include <iostream>
#include <sstream>

void Uci::loop(){
    bool running = false;
    std::string command, option;
    std::unique_ptr<std::thread> st = nullptr;
    SearchTree *tree = SearchTree::GetInstance();
    //std::stringstream lastfen(SearchTree::startFen);
    while(true){
        std::getline(std::cin, command);
        std::stringstream sstr(command);
        sstr >> option;

        // join the searching thread if search has completed without "stop" command
        if(running){
            if(THREAD_READY_TO_JOIN == tree->GetThreadStatus()){
                if(st->joinable()){
                    st->join();
                    tree->SetThreadStatus(THREAD_IDLE);
                    running = false;
                    SearchTree::Init();
                    tree = SearchTree::GetInstance();
                }
            }
        }

        if(option == "uci")
            std::cout << "uciok" << std::endl;
        if(option == "isready")
            std::cout << "readyok" << std::endl;

        if(option == "position"){
            std::string rootstr;
            sstr >> rootstr;
            if(rootstr == "startpos"){
                SearchTree::Init();
            }
            else{
                SearchTree::Init(sstr);
            }
            sstr >> option; // read "moves" word
            while(sstr.good()){
                sstr >> option;
                Move *mov = Move::String2Move(option);
                if(mov != nullptr){
                    if(tree->ForwardTo(mov) == false){
                        sstr.clear();
                        std::cout << "Illegal sequence of moves" << std::endl;
                        delete mov;
                        SearchTree::Init();
                    }
                }
                else{
                    sstr.clear();
                }
            }
        }

        if(option == "go"){
            if(running == false){
                int depth;
                sstr >> depth;
                if(depth == 0){
                    std::cout << "depth must be positive\n";
                    continue;
                }
                running = true;
                tree->SetThreadStatus(THREAD_RUNNING);
                st = std::make_unique<std::thread>(std::thread(executeSearching, depth));
            }
        }

        if(option == "ucinewgame"){
            SearchTree::Init();
        }

        if(option == "stop"){
            if(running){
                tree->SetThreadStatus(THREAD_STOP);
                st->join();
                tree->SetThreadStatus(THREAD_IDLE);
                running = false;
            }
        }

        if(option == "ponderhit"){
            // nothing yet
        }
        if(option == "quit"){
            if(running){
                tree->SetThreadStatus(THREAD_STOP);
                st->join();
            }
            break;
        }

        // not uci
        if(option == "d"){
            tree->ShowBoard();
        }
        if(option == "eval"){
            tree->GetEntryNode()->Evaluate();
        }
        if(option == "hash"){
            std::cout << tree->GetEntryNode()->GetHash() << '\n';
        }
        if(option == "sethash"){
            std::cin >> Evaluator::hashInfo;
        }
    }
    return;
}