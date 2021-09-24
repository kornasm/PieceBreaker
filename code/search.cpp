#include "declarations.h"
#include <stack>
#include "node.h"
#include "search.h"
#include <iostream>
#include "move.h"

void SearchTree::Search(int depth){
    std::stack<Node*> nodesToSearch;
    nodesToSearch.push(entryNode);
    while(!nodesToSearch.empty()){
        Node *searched = nodesToSearch.top();
        nodesToSearch.pop();
        searched->Search(nodesToSearch, depth);
    }
    std::cout << "Eval: " << entryNode->bestval << '\n';
    Node *current = entryNode->bestmove;
    std::cout << "moves  \n";
    while(current){
        std::cout << *(current->moveMade) << '\n';
        current = current->bestmove;
    }
}

void SearchTree::PrintResult(){

}
