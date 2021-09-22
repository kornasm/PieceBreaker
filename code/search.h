#ifndef SEARCH_H_
#define SEARCH_H_

class Node;

class SearchTree{
    public:
        Node *entryNode;
        void Search(int depth);
        void PrintResult();
};

#endif