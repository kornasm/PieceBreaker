#include "search.h"

#include "declarations.h"
#include "move.h"
#include "logger.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

SearchTree* SearchTree::instance = nullptr;
std::string SearchTree::fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string SearchTree::startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

extern Logger logger;

void executeSearching(int depth){
    SearchTree* tree = SearchTree::GetInstance();
    tree->Search(depth);
}

void executeSideThread(){
    SearchTree* tree = SearchTree::GetInstance();
    tree->sideThreadJob();
}
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
        volatile Node *current = entryNode->bestmove;
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
    instance = nullptr;
}

void SearchTree::Init(){
    if(instance != nullptr){
        Clear();
    }
    SearchTree* tree = GetInstance();
    tree->root = new Node();
    tree->entryNode = tree->root;
    tree->status = THREAD_IDLE;
}

void SearchTree::Init(std::stringstream& strFen){
    if(instance != nullptr){
        Clear();
    }
    SearchTree* tree = GetInstance();
    tree->root = new Node(strFen);
    tree->entryNode = tree->root;
    tree->status = THREAD_IDLE;
}

SearchTree* SearchTree::GetInstance(){
    if(!instance){
        instance = new SearchTree();
    }
    return instance;
}

void SearchTree::Clear(){
    SearchTree *tree = GetInstance();
    while(!tree->nodesToSearch.empty()){
        tree->nodesToSearch.pop();
    }
    tree->nodeTable.clear();
    tree->entryNode = nullptr;
    delete tree->root;
    tree->root = nullptr;
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
                searched->Search(maxdepth);
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