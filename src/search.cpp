#include "search.h"

#include "declarations.h"
#include "node.h"
#include "move.h"

#include <stack>
#include <iostream>
#include <iomanip>

SearchTree* SearchTree::instance = NULL;
const std::string SearchTree::startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void executeSearching(int depth){
    SearchTree* tree = SearchTree::GetInstance();
    tree->Search(depth);
}

SearchTree::SearchTree(){
    entryNode = new Node();
}

SearchTree::~SearchTree(void){
    instance = nullptr;
}

void SearchTree::Init(std::string fen){
    Clear();
    SearchTree* tree = GetInstance();
    tree->entryNode = new Node(fen);
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
    delete tree->entryNode;
    tree->entryNode = NULL;
}

void SearchTree::operator()(int maxdepth){
    Search(maxdepth);
}

void SearchTree::Search(int maxdepth){
    //std::stack<Node*> nodesToSearch;
    nodesToSearch.push(entryNode);
    while(!nodesToSearch.empty() && status == THREAD_RUNNING){
        Node *searched = nodesToSearch.front();
        nodesToSearch.pop();
        searched->Search(maxdepth);
    }
    std::cerr << "End Eval: " << entryNode->partialEval << '\n';
    //Explore(entryNode, "");
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
    }//*/
    std::cout << std::endl;
    std::cout << "bestmove " << *(entryNode->bestmove->moveMade);
    if(entryNode->bestmove->bestmove)
        std::cout << " ponder " << *(entryNode->bestmove->bestmove->moveMade);
    std::cout << std::endl;
}

void SearchTree::AddNodeToQueue(Node* node){
    nodesToSearch.push(node);
}

void SearchTree::SetEntry(Node * entry){
    SearchTree* inst = GetInstance();
    inst->entryNode = entry;
}

void SearchTree::ShowBoard(){ 
    std::cout << *entryNode << '\n';
}