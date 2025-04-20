#ifndef DATAITEM_H
#define DATAITEM_H

#include "NodeList.h"
#include <set>
#include <mutex>

class DataItem {
public:
    int value;
    NodeList *nodeList;
    std::set<int> readList;
    std::set<int> writeList;
    std::mutex datalock;

    DataItem();             // Constructor
    ~DataItem();            // Destructor

    Node* addRead(Transaction *t);
    Node* addWrite(Transaction *t);
    void deleteRead(Transaction *t);
    void deleteWrite(Transaction *t);
};

#endif