#ifndef SEARCH_H_
#define SEARCH_H_

#include <queue>
#include <string>
#include <atomic>

class Node;

void executeSearching(int depth);

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
        std::atomic_int status;
        int searchedDepth = 0;
        std::queue<Node*> nodesToSearch;
        Node *entryNode;

    public:
        void Search(int maxdepth);
        void AddNodeToQueue(Node* node);

        int GetThreadStatus() { return status; }
        void SetThreadStatus(int s){ status = s; }

        void PrintResult();
        void ShowBoard();
        Node *GetEntryNode() { return entryNode; }
};

#endif