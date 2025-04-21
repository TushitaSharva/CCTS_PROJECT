#ifndef WAITSFORGRAPH_H
#define WAITSFORGRAPH_H

#include <set>
#include <unordered_map>
#include <unordered_set>
#include "DataItem.h"

class WaitsForGraph {
public:
    std::set<int> vertices;
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;

    WaitsForGraph() = default;
    ~WaitsForGraph();
    bool addReadOperation(int transactionId, DataItem* item, OperationType type);
    bool addWriteOperation(int transactionId, DataItem* item, OperationType type);
    bool detectCycleUtil(int node, std::unordered_set<int>&visited, std::unordered_set<int> &recStack);
    bool detectCycle();
    void garbageCollect();
};

#endif