#ifndef WAITSFORGRAPH_H
#define WAITSFORGRAPH_H

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "DataItem.h"

class WaitsForGraph {
public:
    // Set of all known transaction IDs
    std::set<int> vertices;

    // Waits-for graph represented as an adjacency list
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;

    WaitsForGraph() = default;
    ~WaitsForGraph();

    bool addReadOperation(int transactionId, DataItem* item);
    bool addWriteOperation(int transactionId, DataItem* item);
    void printAdjList(const std::unordered_map<int, std::unordered_set<int>>& adjList) const;

    // Updated to include path tracking
    bool detectCycle(const std::unordered_map<int, std::unordered_set<int>>& tempAdjList);
    bool detectCycleUtil(int node,
                         std::unordered_set<int>& visited,
                         std::unordered_set<int>& recStack,
                         const std::unordered_map<int, std::unordered_set<int>>& graph,
                         std::vector<int>& path);

    void garbageCollect();
};

#endif
