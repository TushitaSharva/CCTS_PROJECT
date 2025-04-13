#ifndef WAITSFORGRAPH_H
#define WAITSFORGRAPH_H

#include <set>
#include <vector>
#include "OperationType.h"

class WaitsForGraph {
public:
    std::set<int> vertices;
    std::vector<std::vector<int>> adjacencyList; // conflicts

    bool addOperation(int transactionId, int dataItemIndex, OperationType type);
    bool detectCycle();
    void garbageCollect();
};

#endif