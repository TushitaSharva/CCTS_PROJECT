#include "WaitsForGraph.h"
#include <iostream>
#include "Logger.h"
#include <vector>
#include <algorithm>

static Logger LOGGER;

WaitsForGraph::~WaitsForGraph()
{
    vertices.clear();
    adjacencyList.clear();
}

void WaitsForGraph::printAdjList(const std::unordered_map<int, std::unordered_set<int>> &adjList) const
{
    for (const auto &pair : adjList)
    {
        std::cout << "T" << pair.first << " -> ";
        for (int neighbor : pair.second)
        {
            std::cout << "T" << neighbor << " ";
        }
        std::cout << "\n";
    }
}

bool WaitsForGraph::addReadOperation(int transactionId, DataItem *item)
{
    auto tempAdjList = adjacencyList;

    for (int writeTransaction : item->writeList)
    {
        if (writeTransaction != transactionId)
        {
            tempAdjList[writeTransaction].insert(transactionId);
        }
    }

    // std::cout << "Before checking cycle:\n";
    // printAdjList(tempAdjList);

    if (!detectCycle(tempAdjList))
    {
        for (int writeTransaction : item->writeList)
        {
            if (writeTransaction != transactionId)
            {
                adjacencyList[writeTransaction].insert(transactionId);
            }
        }

        // Track all seen transactions
        vertices.insert(transactionId);
        for (int t : item->writeList)
            vertices.insert(t);

        // std::cout << "Final adjList after insertion:\n";
        // printAdjList(adjacencyList);

        return true;
    }

    return false;
}

bool WaitsForGraph::addWriteOperation(int transactionId, DataItem *item)
{
    auto tempAdjList = adjacencyList;

    for (int writeTransaction : item->writeList)
    {
        if (writeTransaction != transactionId)
        {
            tempAdjList[writeTransaction].insert(transactionId);
        }
    }

    for (int readTransaction : item->readList)
    {
        if (readTransaction != transactionId)
        {
            tempAdjList[readTransaction].insert(transactionId);
        }
    }

    // std::cout << "Before checking cycle:\n";
    // printAdjList(tempAdjList);

    if (!detectCycle(tempAdjList))
    {
        for (int writeTransaction : item->writeList)
        {
            if (writeTransaction != transactionId)
            {
                adjacencyList[writeTransaction].insert(transactionId);
            }
        }

        for (int readTransaction : item->readList)
        {
            if (readTransaction != transactionId)
            {
                adjacencyList[readTransaction].insert(transactionId);
            }
        }

        vertices.insert(transactionId);
        for (int t : item->writeList)
            vertices.insert(t);
        for (int t : item->readList)
            vertices.insert(t);

        // std::cout << "Final adjList after insertion:\n";
        // printAdjList(adjacencyList);

        return true;
    }

    return false;
}

bool WaitsForGraph::detectCycleUtil(int node, std::unordered_set<int> &visited, std::unordered_set<int> &recStack, const std::unordered_map<int, std::unordered_set<int>> &graph, std::vector<int> &path)
{
    visited.insert(node);
    recStack.insert(node);
    path.push_back(node);

    auto it = graph.find(node);
    if (it != graph.end())
    {
        for (int neighbor : it->second)
        {
            if (recStack.count(neighbor))
            {
                // Cycle detected, print path
                auto it = std::find(path.begin(), path.end(), neighbor);
                std::string str = "Cycle detected: ";
                while (it != path.end())
                {
                    str += "T"+std::to_string(*it)+" -> ";
                    ++it;
                }
                str += "T" + std::to_string(neighbor);
                LOGGER.OUTPUTT(str);
                return true;
            }
            if (!visited.count(neighbor))
            {
                if (detectCycleUtil(neighbor, visited, recStack, graph, path))
                    return true;
            }
        }
    }

    recStack.erase(node);
    path.pop_back();
    return false;
}

bool WaitsForGraph::detectCycle(const std::unordered_map<int, std::unordered_set<int>> &tempAdjList)
{
    std::unordered_set<int> visited, recStack;
    std::vector<int> path;

    for (const auto &[node, _] : tempAdjList)
    {
        if (!visited.count(node))
        {
            if (detectCycleUtil(node, visited, recStack, tempAdjList, path))
            {
                return true;
            }
        }
    }
    return false;
}

void WaitsForGraph::garbageCollect()
{
    // Optional: remove transactions that are committed/aborted from adjacencyList & vertices
}

void WaitsForGraph::deleteNode(int transactionId) {
    vertices.erase(transactionId);
    adjacencyList.erase(transactionId);
    for (auto& [node, neighbors] : adjacencyList) {
        neighbors.erase(transactionId);
    }
}

