#include "search.h"

#include "declarations.h"
#include "move.h"
#include "logger.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

extern Logger logger;

void SearchTree::sideThreadJob(){
    Logger sideThrLog(LOG_UCI);
    auto start = std::chrono::steady_clock::now();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(!nodesToSearch.empty() && status == THREAD_RUNNING){
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        //auto millis = std::chrono::duration_cast<
        sideThrLog << LogDest(LOG_UCI) << "info ";
        sideThrLog << "nodes " << noNodes << " ";
        double secs = (double)(std::chrono::duration_cast<std::chrono::seconds>(diff).count());
        int nps = (int)((double)(noNodes) / secs);
        Node *current = entryNode->bestmove;
        sideThrLog << "nps " << nps << " ";
        sideThrLog << "time " << diff.count() << " ";
        sideThrLog << "pv ";
        
        
        while(current){
            sideThrLog << *(current->moveMade) << " ";
            current = current->bestmove;
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

SearchTree::SearchTree() :nodesToSearch(&CompareNodesAscending) {}

SearchTree::~SearchTree(void){
    Clear();
}

void SearchTree::Init(){
    if(initialized)
        Clear();
    initialized = true;

    SearchTree* tree = this;
    root = new Node();
    entryNode = tree->root;
    status = THREAD_IDLE;
}

void SearchTree::Init(std::stringstream& strFen){
    if(initialized)
        Clear();
    initialized = true;

    root = new Node(strFen);
    entryNode = root;
    status = THREAD_IDLE;
}

void SearchTree::Clear(){
    while(!nodesToSearch.empty()){
        nodesToSearch.pop();
    }

    nodeTable.clear();
    entryNode = nullptr;
    delete root;
    root = nullptr;
}

bool SearchTree::ForwardTo(Move *move){
    if(Node *next = entryNode->CheckMove(move)){
        entryNode = next;
        entryNode->Isolate();
        return true;
    }
    else{
        return false;
    }
}

void SearchTree::Search(int maxdepth){
    nodesToSearch.push(entryNode);
    nodeTable.insert(std::make_pair(entryNode, NODE_NOT_SEARCHED));
    while(!nodesToSearch.empty() && status == THREAD_RUNNING){
        Node *searched = nodesToSearch.top();
        auto it = nodeTable.find(searched);
        if(it == nodeTable.end()){
            logger << LogDest(LOG_ERROR) << "Trying to search a node not found in node table\n";
            nodeTable.insert(std::make_pair(searched, 0));
        }
        else{
            nodesToSearch.pop();
            if(it->second == 0){
                searched->Search(this, maxdepth);
                Explore(searched, "", 0);
                it->second = NODE_SEARCHED;
            }
            else if(it->second == NODE_SEARCHED){
                logger << LogDest(LOG_ERROR) << "Searching a node second time\n";
                Explore(searched, "", 0, 1);

                assert(0 == 1 && "searching a node the second time");
            }
            else if(it->second == NODE_DELETED){
                logger << LogDest(LOG_DEBUG) << "node table erasing:  " << it->first << "\n";
                nodeTable.erase(it->first);
            }
        }
    }
    logger << LogDest(LOG_ANALYSIS) << "End Eval: " << entryNode->searchEval << '\n';
    Explore(entryNode, "", 10, 0);
    PrintResult();
    //Clear();
    status = THREAD_READY_TO_JOIN;
}

void SearchTree::PrintResult(int level) const{
    logger << LogDest(level) << "Full path  : ";
    Node *current = entryNode->bestmove;
    while(current){
        logger << LogDest(level) << *(current->moveMade) << ' ';
        current = current->bestmove;
    }
    logger << LogDest(level) << "\n";
    logger << LogDest(LOG_UCI) << "bestmove " << *(entryNode->bestmove->moveMade);
    if(entryNode->bestmove->bestmove)
        logger << LogDest(LOG_UCI) << " ponder " << *(entryNode->bestmove->bestmove->moveMade);
    logger << LogDest(LOG_ALWAYS) << "\n";
}

void SearchTree::AddNodeToQueue(Node* node){
    nodesToSearch.push(node);
    nodeTable.insert(std::make_pair(node, 0));
}

void SearchTree::ShowBoard(int level) const{
    logger << LogDest(level) << *entryNode << '\n';
    logger << LogDest(level) << entryNode->GetFen() << '\n';
}