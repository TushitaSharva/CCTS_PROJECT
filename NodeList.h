#ifndef NODELIST_H
#define NODELIST_H

#include <set>
#include "Node.h"

class NodeList {
public:
    Node *head;
    Node *tail;

    void addReadNode(Transaction *t);
    void addWriteNode(Transaction *t);
    void deleteWriteNode(Transaction *t);
    void deleteWriteNode(Transaction *t);
    Node *getHead();
    Node *getTail();    
};

#endif