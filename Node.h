#ifndef NODE_H
#define NODE_H

#include <set>
#include "Transaction.h"
#include "OperationType.h"

class Node {
public:
    Node *next;
    OperationType type;
};

class ReadNode : public Node {
public:
    std::set<Transaction> tlist;
    ReadNode() {
        type = READ;
    }
};

class WriteNode : public Node {
public:
    Transaction *transaction;
    WriteNode() {
        type = WRITE;
    }
};

#endif