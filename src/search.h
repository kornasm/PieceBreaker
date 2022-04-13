#ifndef SEARCH_H_
#define SEARCH_H_

#include "node.h"

#include <queue>
#include <string>
#include <atomic>
#include <set>
#include <map>

class Node;

class NodePtr{
    public:
        const Node* ptr;
        int status; // 0 - not searched, 1 - searched, 2 - deleted
};

void executeSearching(int depth);

class SearchTree{
    private:
        static SearchTree* instance;
        SearchTree();

    public:
        static const std::string startFen;
        static std::string fen;
        static void Init(std::string fen = SearchTree::startFen);
        static SearchTree* GetInstance();
        static void Clear();
        ~SearchTree(void);

        std::map<Node*, int> nodeTable;

    private:
        std::atomic_int status;
        int searchedDepth = 0;
        std::priority_queue<Node*, std::vector<Node*>, decltype(&CompareNodesDescending)> nodesToSearch;
        Node *entryNode;
        Node *root;

    public:
        bool ForwardTo(Move *move);
        void Search(int maxdepth);
        void AddNodeToQueue(Node* node);

        int GetThreadStatus() { return status; }
        void SetThreadStatus(int s){ status = s; }

        void PrintResult();
        void ShowBoard();
        Node *GetEntryNode() { return entryNode; }
};

#endif