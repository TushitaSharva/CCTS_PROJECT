#include "WaitsForGraph.h"
#include <iostream>

WaitsForGraph::~WaitsForGraph() {
    vertices.clear();
    adjacencyList.clear();
}

bool WaitsForGraph::addReadOperation(int transactionId, DataItem *item, OperationType type) {
    auto tempAdjList = adjacencyList;

    for (auto writeTransaction : item->writeList) {
        tempAdjList[writeTransaction].insert(transactionId);
    }

    if (!detectCycle(tempAdjList)) {
        for(int writeTransaction: item->writeList) {
            adjacencyList[writeTransaction].insert(transactionId);
        }
        return true;
    }

    return false;
}

bool WaitsForGraph::addWriteOperation(int transactionId, DataItem *item, OperationType type) {
    auto tempAdjList = adjacencyList;

    for (auto writeTransaction : item->writeList) {
        tempAdjList[writeTransaction].insert(transactionId);
    }

    for (auto readTransaction : item->readList) {
        if(transactionId != readTransaction) {
            tempAdjList[readTransaction].insert(transactionId);
        }
    }

    if (!detectCycle(tempAdjList)) {
        for (int writeTransaction : item->writeList) {
            adjacencyList[writeTransaction].insert(transactionId);
        }

        for (int readTransaction : item->readList) {
            adjacencyList[readTransaction].insert(transactionId);
        }
        return true;
    }

    return false;
}

bool WaitsForGraph::detectCycleUtil(int node, std::unordered_set<int>& visited, std::unordered_set<int>& recStack) {
    visited.insert(node);
    recStack.insert(node);

    for (int neighbor : adjacencyList[node]) {
        if (recStack.count(neighbor)) return true;
        if (!visited.count(neighbor) && detectCycleUtil(neighbor, visited, recStack))
            return true;
    }

    recStack.erase(node);
    return false;
}

bool WaitsForGraph::detectCycle(const std::unordered_map<int, std::unordered_set<int>>&tempAdjList) {
    std::unordered_set<int> visited, recStack;
    for (const auto& [node, _] : adjacencyList) {
        if (!visited.count(node)) {
            if (detectCycleUtil(node, visited, recStack)) {
                return true;
            }
        }
    }
    return false;
}


void WaitsForGraph::garbageCollect() {
    
}
