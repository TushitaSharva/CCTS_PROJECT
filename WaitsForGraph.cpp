#include "WaitsForGraph.h"
#include <iostream>

WaitsForGraph::~WaitsForGraph() {
    vertices.clear();
    adjacencyList.clear();
}

void WaitsForGraph::printAdjList(const std::unordered_map<int, std::unordered_set<int>> &adjList) const {
    for (const auto &pair : adjList) {
        std::cout << "T" << pair.first << " -> ";
        for (int neighbor : pair.second) {
            std::cout << "T" << neighbor << " ";
        }
        std::cout << "\n";
    }
}

bool WaitsForGraph::addReadOperation(int transactionId, DataItem *item) {
    auto tempAdjList = adjacencyList;

    for (int writeTransaction : item->writeList) {
        if (writeTransaction != transactionId) {
            tempAdjList[writeTransaction].insert(transactionId);
        }
    }

    // std::cout << "Before checking cycle:\n";
    // printAdjList(tempAdjList);

    if (!detectCycle(tempAdjList)) {
        for (int writeTransaction : item->writeList) {
            if (writeTransaction != transactionId) {
                adjacencyList[writeTransaction].insert(transactionId);
            }
        }

        // Track all seen transactions
        vertices.insert(transactionId);
        for (int t : item->writeList) vertices.insert(t);

        // std::cout << "Final adjList after insertion:\n";
        // printAdjList(adjacencyList);

        return true;
    }

    return false;
}

bool WaitsForGraph::addWriteOperation(int transactionId, DataItem *item) {
    auto tempAdjList = adjacencyList;

    for (int writeTransaction : item->writeList) {
        if (writeTransaction != transactionId) {
            tempAdjList[writeTransaction].insert(transactionId);
        }
    }

    for (int readTransaction : item->readList) {
        if (readTransaction != transactionId) {
            tempAdjList[readTransaction].insert(transactionId);
        }
    }

    // std::cout << "Before checking cycle:\n";
    // printAdjList(tempAdjList);

    if (!detectCycle(tempAdjList)) {
        for (int writeTransaction : item->writeList) {
            if (writeTransaction != transactionId) {
                adjacencyList[writeTransaction].insert(transactionId);
            }
        }

        for (int readTransaction : item->readList) {
            if (readTransaction != transactionId) {
                adjacencyList[readTransaction].insert(transactionId);
            }
        }

        vertices.insert(transactionId);
        for (int t : item->writeList) vertices.insert(t);
        for (int t : item->readList) vertices.insert(t);

        // std::cout << "Final adjList after insertion:\n";
        // printAdjList(adjacencyList);

        return true;
    }

    return false;
}

bool WaitsForGraph::detectCycleUtil(int node, std::unordered_set<int>& visited, std::unordered_set<int>& recStack,
                                    const std::unordered_map<int, std::unordered_set<int>> &graph) {
    visited.insert(node);
    recStack.insert(node);

    auto it = graph.find(node);
    if (it != graph.end()) {
        for (int neighbor : it->second) {
            if (recStack.count(neighbor)) return true;
            if (!visited.count(neighbor) && detectCycleUtil(neighbor, visited, recStack, graph)) return true;
        }
    }

    recStack.erase(node);
    return false;
}

bool WaitsForGraph::detectCycle(const std::unordered_map<int, std::unordered_set<int>>& tempAdjList) {
    std::unordered_set<int> visited, recStack;
    for (const auto& [node, _] : tempAdjList) {
        if (!visited.count(node)) {
            if (detectCycleUtil(node, visited, recStack, tempAdjList)) {
                return true;
            }
        }
    }
    return false;
}

void WaitsForGraph::garbageCollect() {
    // Optional: remove transactions that are committed/aborted from adjacencyList & vertices
}
