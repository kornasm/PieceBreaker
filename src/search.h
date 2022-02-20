#ifndef SEARCH_H_
#define SEARCH_H_

#define THREAD_IDLE 0 
#define THREAD_RUNNING 1
#define THREAD_STOP 2
#define THREAD_READY_TO_JOIN 3

#include <unordered_map>
#include <queue>
#include <string>
#include <atomic>

class Node;

void executeSearching(int depth);

class SearchTree{
    public:
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
        std::unordered_map<long long, Node*> globalmap;
        int searchedDepth = 0;
        std::queue<Node*> nodesToSearch;
        Node *entryNode;

    public:
        int GetThreadStatus() { return status; }
        void SetThreadStatus(int s){ status = s; }

        void operator()(int maxdepth);
        void Search(int maxdepth);
        void PrintResult();
        void AddNodeToQueue(Node* node);
        void SetEntry(Node * entry);
        void ShowBoard();
        Node *GetEntryNode() {return entryNode; }
};

#endif