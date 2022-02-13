#include "declarations.h"
#include <stack>
#include "node.h"
#include "search.h"
#include <iostream>
#include "move.h"

SearchTree* SearchTree::instance = NULL;

SearchTree::~SearchTree(void){
    //delete entryNode;
    instance = nullptr;
}

void SearchTree::Search(int maxdepth){
    //std::stack<Node*> nodesToSearch;
    nodesToSearch.push(entryNode);
    while(!nodesToSearch.empty()){
        Node *searched = nodesToSearch.front();
        nodesToSearch.pop();
        searched->Search(maxdepth);
    }
    std::cout << "End Eval: " << entryNode->partialEval << '\n';
    PrintResult();
}

void SearchTree::PrintResult(){
    Node *current = entryNode->bestmove;
    std::cout << "moves  \n";
    while(current){
        std::cout << *(current->moveMade) << '\n';
        current = current->bestmove;
    }
}

SearchTree* SearchTree::GetInstance(){
    if(!instance){
        instance = new SearchTree();
    }
    return instance;
}

SearchTree* SearchTree::ResetAndGet(){
    delete instance;
    instance = NULL;
    return GetInstance();
}

void SearchTree::AddNodeToQueue(Node* node){
    nodesToSearch.push(node);
}

void SearchTree::SetEntry(Node * entry){
    SearchTree* inst = GetInstance();
    inst->entryNode = entry;
}