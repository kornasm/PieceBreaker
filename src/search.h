#ifndef SEARCH_H_
#define SEARCH_H_

#include <unordered_map>
#include <queue>
#include <string>

class Node;

class SearchTree{
    private:
        static SearchTree* instance;
        SearchTree();

    public:
        static const std::string startFen;
        static void Init(std::string fen = SearchTree::startFen);
        static SearchTree* GetInstance();
        static void Clear();
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
        void ShowBoard();
        Node *GetEntryNode() {return entryNode; }
};

#endif