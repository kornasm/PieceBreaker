#include "declarations.h"
#include "node.h"
#include "position.h"
#include "move.h"

Node::Node(){
    position = new Position();
}

Node::Node(std::string fen){
    position = new Position(fen);
}

Node::Node(Position *pos, Node* prev){
    position = pos;
    if(prev){
        depth = prev->depth + 1;
    }
}

Node::~Node(){
    delete position;
    for(auto node : children){
        delete node;
    }
}

bool Node::CheckMove(Move *move){
    Position *newposition = position->MakeMove(move);
    if(newposition){
        children.push_back(new Node(newposition, this));
        return true;
    }
    return false;
}

void Node::GenerateChildNodes(){
    
}