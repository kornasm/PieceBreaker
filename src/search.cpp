#include "search.h"

#include "declarations.h"
#include "node.h"
#include "move.h"

#include <stack>
#include <iostream>
#include <iomanip>

SearchTree* SearchTree::instance = NULL;
const std::string SearchTree::startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

SearchTree::SearchTree(){
    //entryNode = new Node();
}

SearchTree::~SearchTree(void){
    instance = nullptr;
}

void SearchTree::Init(std::string fen){
    Clear();
    SearchTree* tree = GetInstance();
    tree->entryNode = new Node(fen);
}

SearchTree* SearchTree::GetInstance(){
    if(!instance){
        instance = new SearchTree();
    }
    return instance;
}

void SearchTree::Clear(){
    SearchTree *tree = GetInstance();
    delete tree->entryNode;
    tree->entryNode = NULL;
}

void Explore(Node *nd, std::string prefix){
    std::cerr << prefix  << "NODE " << nd << "\tprev " << nd->prev << "\tdepth " << nd->depth << "\teval " << nd->partialEval << "\tbest " << nd->bestmove << "\t";
    if(nd->moveMade){
        std::cerr << *(nd->moveMade);
    }
    std::cerr << '\n';
    for(auto node : nd->children){
        Explore(node, prefix + '\t');
    }
    return;
}

void SearchTree::Search(int maxdepth){
    //std::stack<Node*> nodesToSearch;
    nodesToSearch.push(entryNode);
    while(!nodesToSearch.empty()){
        Node *searched = nodesToSearch.front();
        nodesToSearch.pop();
        searched->Search(maxdepth);
    }
    std::cerr << "End Eval: " << entryNode->partialEval << '\n';
    Explore(entryNode, "");
    PrintResult();
}

void SearchTree::PrintResult(){
    /*Node *current = entryNode->bestmove;
    std::cout << "moves  \n";
    while(current){
        std::cout << *(current->moveMade) << '\n';
        current = current->bestmove;
    }//*/
    std::cout << "bestmove " << *(entryNode->bestmove->moveMade) << " ponder " << *(entryNode->bestmove->bestmove->moveMade) << '\n';

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