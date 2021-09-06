#include "declarations.h"

Node::Node(){
    position = new Position();
}

Node::Node(std::string fen){
    position = new Position(fen);
}

Node::Node(Position *pos){
    position = pos;
}

Node::~Node(){
    delete position;
    for(auto node : children){
        delete node;
    }
}

bool Node::CheckMove(Move *move){
    Position *newposition = position->CheckMove(move);
    if(newposition){
        children.push_back(new Node(newposition));
        return true;
    }
    return false;
}