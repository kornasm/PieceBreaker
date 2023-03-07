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

#define NODE_NOT_SEARCHED 0
#define NODE_SEARCHED 1
#define NODE_DELETED 2

class NodePtr{
    public:
        const Node* ptr;
        int status; // 0 - not searched, 1 - searched, 2 - deleted
};

class SearchTree{
    private:
        std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        const std::string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        bool initialized = 0;
        std::map<Node*, int> nodeTable; // map storing node status (not searched, searched, to delete)
                                        // used to avoid memory problems (dereferencing deleted nodes, double frees)

        std::atomic_int status;
        int searchedDepth = 0;
        std::priority_queue<Node*, std::vector<Node*>, decltype(&CompareNodesDescending)> nodesToSearch;
        Node *entryNode;
        Node *root;
        long long operationNumber = 0;

    public:
        SearchTree();
        void Init();
        void Init(std::stringstream& strFen);

        ~SearchTree(void);
        void Clear();

        // stats
        int noNodes = 0;

    public:
        bool ForwardTo(Move *move);
        void Search(int maxdepth);
        void sideThreadJob();
        void AddNodeToQueue(Node* node);

        int GetThreadStatus() { return status; }
        void SetThreadStatus(int s){ status = s; }

        void PrintResult(int level = LOG_ANALYSIS) const;
        void ShowBoard(int level = LOG_ANALYSIS) const;
        Node *GetEntryNode() const { return entryNode; }

        //stats
        inline void IncreaseNodeCount() { noNodes++; }
        
};

#endif