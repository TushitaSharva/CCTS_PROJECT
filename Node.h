#ifndef NODE_H
#define NODE_H

#include <set>
#include <mutex>
#include <condition_variable>
#include "Transaction.h"
#include "OperationType.h"
#include <iostream>

class Node {
public:
    Node *next;
    OperationType type;

    // Variables for signalling waiting threads
    std::mutex mtx;
    std::condition_variable cv;
    bool isAtHead;

    Node(OperationType t) : type(t) {
        next = nullptr;
        isAtHead = false;
    }
    virtual ~Node() {}
};

class ReadNode : public Node {
public:
    std::set<Transaction*> tlist;

    ReadNode() : Node(READ) {}
    ~ReadNode() override = default;
};

class WriteNode : public Node {
public:
    Transaction *transaction;
    
    WriteNode(Transaction* t) : Node(WRITE), transaction(t) {}
    ~WriteNode() override = default;
};

#endif