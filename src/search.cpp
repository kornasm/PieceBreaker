#include "search.h"

#include "declarations.h"
#include "move.h"
#include "logger.h"

#include <iostream>

SearchTree* SearchTree::instance = nullptr;
std::string SearchTree::fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string SearchTree::startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

extern Logger logger;

void executeSearching(int depth){
    SearchTree* tree = SearchTree::GetInstance();
    tree->Search(depth);
}

SearchTree::SearchTree() :nodesToSearch(&CompareNodesAscending) {}

SearchTree::~SearchTree(void){
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
    nodeTable.insert(std::make_pair(entryNode, 0));
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
                it->second = 1;
            }
            else if(it->second == 1){
                logger << LogDest(LOG_ERROR) << "Searching a node second time\n";
                Explore(searched, "", 0, 1);
                exit(EXIT_SUCCESS); // just to see if it happens
            }
            else if(it->second == 2){
                logger << LogDest(LOG_DEBUG) << "node table erasing:  " << it->first << "\n";
                nodeTable.erase(it->first);
            }
        }
    }
    logger << LogDest(LOG_ANALYSIS) << "End Eval: " << entryNode->partialEval << '\n';
    Explore(entryNode, "", 10, 0);
    PrintResult();
    Clear();
    status = THREAD_READY_TO_JOIN;
}

void SearchTree::PrintResult(int level){
    logger << LogDest(level) << "Full path  : ";
    Node *current = entryNode->bestmove;
    while(current){
        logger << LogDest(level) << *(current->moveMade) << ' ';
        current = current->bestmove;
    }
    logger << LogDest(LOG_UCI) << "\nbestmove " << *(entryNode->bestmove->moveMade);
    if(entryNode->bestmove->bestmove)
        logger << LogDest(LOG_UCI) << " ponder " << *(entryNode->bestmove->bestmove->moveMade);
    logger << LogDest(LOG_ALWAYS) << "\n";
}

void SearchTree::AddNodeToQueue(Node* node){
    nodesToSearch.push(node);
    nodeTable.insert(std::make_pair(node, 0));
}

void SearchTree::ShowBoard(int level){ 
    logger << LogDest(level) << *entryNode << '\n';
    logger << LogDest(level) << entryNode->GetFen() << '\n';
    logger << LogDest(level) << *root << '\n';
}