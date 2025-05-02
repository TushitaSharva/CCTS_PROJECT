#ifndef NODELIST_H
#define NODELIST_H

#include <set>
#include "Node.h"

class NodeList {
public:
    Node *head;
    Node *tail;
    // std::mutex listMutex;

    NodeList();
    ~NodeList();
    Node* addReadNode(Transaction *t);
    Node* addWriteNode(Transaction *t);
    void deleteReadNode(Transaction *t);
    void deleteWriteNode(Transaction *t);   
};

#endif