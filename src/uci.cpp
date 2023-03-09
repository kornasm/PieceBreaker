#include "uci.h"

#include "search.h"
#include "evaluate.h"
#include "position.h"
#include "move.h"
#include "input_provider.h"
#include "logger.h"

#include <thread>
#include <iostream>
#include <sstream>
#include <chrono>

extern Logger logger;

Uci::Uci(){
    tree = std::unique_ptr<SearchTree>(new SearchTree());
    tree->Init();
    ip = InputProvider::GetInstance();
}

void Uci::UciConfirm(){
    logger << LogDest(LOG_ALWAYS) << "uciok\n";
}

void Uci::IsReady(){
    logger << LogDest(LOG_ALWAYS) << "readyok\n";
}
void Uci::Position(){
    std::string rootstr;
    sstr >> rootstr;
    if(rootstr == "startpos"){
        tree->Init();
    }
    else{
        tree->Init(sstr);
    }
    sstr >> option; // read "moves" word
    while(sstr.good()){
        sstr >> option;
        Move *mov = Move::String2Move(option);
        if(mov != nullptr){
            if(tree->ForwardTo(mov) == false){
                sstr.clear();
                logger << LogDest(LOG_ERROR) << "Illegal sequence of moves\n";
                delete mov;
                tree->Init();
            }
        }
        else{
            sstr.clear();
        }
    }
}

void Uci::Go(){
    if(running == false){
        int depth;
        sstr >> depth;
        if(depth == 0){
            logger <<LogDest(LOG_ERROR) << "depth must be positive\n";
            return;
        }
        running = true;
        tree->SetThreadStatus(THREAD_RUNNING);
        st = std::make_unique<std::thread>(std::thread(&SearchTree::Search, tree.get(), depth));
        side_thr = std::make_unique<std::thread>(std::thread(&SearchTree::sideThreadJob, tree.get()));
    }
}


void Uci::UciNewGame(){
    tree->Init();
}
void Uci::Stop(){
    if(running){
        tree->SetThreadStatus(THREAD_STOP);
        st->join();
        side_thr->join();
        tree->SetThreadStatus(THREAD_IDLE);
        running = false;
    }
}

void Uci::Quit(){
    Stop();
    loop = false;
}
void Uci::Loop(){

    while(loop){

        sstr.clear();
        sstr << ip->GetNextCommand();
        logger << LogDest(LOG_DEBUG) << sstr.str() << "\n";
        sstr >> option;
        
        if(option == "explore")
            ExploreRoot();
        

        JoinThreads();


        if(option == "uci")
            UciConfirm();
        if(option == "isready")
            IsReady();

        if(option == "position")
            Position();
        if(option == "go")
            Go();
        if(option == "ucinewgame")
            UciNewGame();
        if(option == "stop")
            Stop();

        //if(option == "ponderhit"){}
            // nothing yet
        if(option == "quit" || option == "q")
            Quit();


        if(option == "d")
            ShowBoard();
        if(option == "eval")
            Eval();
        
        if(option == "hash")
            GetHash();
        if(option == "sethash")
            SetHash();
        
#ifdef DEBUG
        if(option == "sleep"){
            Sleep();
        }
#endif
    }
    return;
}

void Uci::ExploreRoot(){
    int depth = 1;
    sstr >> depth;
    Explore(tree->GetEntryNode(), "", depth, LOG_ALWAYS);
}

void Uci::ShowBoard(){
    tree->ShowBoard(LOG_ALWAYS);
}
void Uci::Eval(){
    tree->GetEntryNode()->Evaluate();
    logger << LogDest(LOG_ALWAYS) << tree->GetEntryNode()->GetEval() << "\n";
}

void Uci::GetHash(){
    logger << LogDest(LOG_ALWAYS) << tree->GetEntryNode()->GetHash() << '\n';
}
void Uci::SetHash(){
    sstr >> Evaluator::hashInfo;
}

void Uci::Sleep(){
    int millis;
    sstr >> millis;
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Uci::JoinThreads(){
    if(running){
        if(THREAD_READY_TO_JOIN == tree->GetThreadStatus()){
            if(st->joinable()){
                st->join();
                tree->SetThreadStatus(THREAD_IDLE);
                running = false;
                tree->Init();
            }
            if(side_thr->joinable()){
                side_thr->join();
            }
        }
    }
}