#ifndef SEARCH_H_
#define SEARCH_H_

#include <unordered_map>
#include <queue>

class Node;

class SearchTree{
    private:
        static SearchTree* instance;
        SearchTree(){};
    public:    
        static SearchTree* GetInstance();
        static SearchTree* ResetAndGet();
        ~SearchTree(void);
    private:
        std::unordered_map<long long, Node*> globalmap;
        int searchedDepth = 0;
        std::queue<Node*> nodesToSearch;
        Node *entryNode;
    public:
        void Search(int maxdepth);
        void PrintResult();
        void AddNodeToQueue(Node* node);
        void SetEntry(Node * entry);
};

#endif