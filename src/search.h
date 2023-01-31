#ifndef SEARCH_H_
#define SEARCH_H_

#include "node.h"
#include "logger.h"

#include <queue>
#include <string>
#include <atomic>
#include <set>
#include <map>

class Node;
class Logger;

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
        static void Init();
        static void Init(std::stringstream& strFen);
        static SearchTree* GetInstance();
        static void Clear();
        ~SearchTree(void);

        std::map<Node*, int> nodeTable; // map storing node status (not searched, searched, to delete)
                                        // used to avoid memory problems (dereferencing deleted nodes, double frees)

    private:
        std::atomic_int status;
        int searchedDepth = 0;
        std::priority_queue<Node*, std::vector<Node*>, decltype(&CompareNodesDescending)> nodesToSearch;
        Node *entryNode;
        Node *root;
        long long operationNumber = 0;

    public:
        bool ForwardTo(Move *move);
        void Search(int maxdepth);
        void AddNodeToQueue(Node* node);

        int GetThreadStatus() { return status; }
        void SetThreadStatus(int s){ status = s; }

        void PrintResult(int level = LOG_ANALYSIS);
        void ShowBoard(int level = LOG_ANALYSIS);
        Node *GetEntryNode() { return entryNode; }
};

#endif