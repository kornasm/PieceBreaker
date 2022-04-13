#include "search.h"

#include "declarations.h"
#include "move.h"

#include <iostream>

SearchTree* SearchTree::instance = nullptr;
std::string SearchTree::fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string SearchTree::startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void executeSearching(int depth){
    SearchTree* tree = SearchTree::GetInstance();
    tree->Search(depth);
}

SearchTree::SearchTree() :nodesToSearch(&CompareNodesAscending) {}

SearchTree::~SearchTree(void){
    instance = nullptr;
}

void SearchTree::Init(std::string fen){
    if(instance != nullptr){
        Clear();
    }
    SearchTree* tree = GetInstance();
    tree->root = new Node(fen);
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
            std::cout << "Trying to search a node not found in node table" << std::endl;
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
                std::cout << "Searching a node second time    " << std::endl;
                Explore(searched, "", 0, 1);
                exit(EXIT_SUCCESS); // just to see if it happens
            }
            else if(it->second == 2){
                std::cout << "node table erasing:  " << it->first << std::endl;
                nodeTable.erase(it->first);
            }
        }
    }
    std::cerr << "End Eval: " << entryNode->partialEval << '\n';
    Explore(entryNode, "", 10, 0);
    PrintResult();
    Clear();
    status = THREAD_READY_TO_JOIN;
}

void SearchTree::PrintResult(){
    std::cout << "Full path  : ";
    Node *current = entryNode->bestmove;
    while(current){
        std::cout << *(current->moveMade) << ' ';
        current = current->bestmove;
    }
    std::cout << "\nbestmove " << *(entryNode->bestmove->moveMade);
    if(entryNode->bestmove->bestmove)
        std::cout << " ponder " << *(entryNode->bestmove->bestmove->moveMade);
    std::cout << std::endl;
}

void SearchTree::AddNodeToQueue(Node* node){
    nodesToSearch.push(node);
    nodeTable.insert(std::make_pair(node, 0));
}

void SearchTree::ShowBoard(){ 
    std::cout << *entryNode << '\n';
    std::cout << *root << '\n';
}