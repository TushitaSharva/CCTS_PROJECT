#ifndef DATAITEM_H
#define DATAITEM_H

#include "NodeList.h"
#include <set>
#include <mutex>

class DataItem {
    int value;
    NodeList *nodeList;
    std::set<int> readList;
    std::set<int> writeList;
    std::mutex datalock;

    void addRead(Transaction *t);
    void addWrite(Transaction *t);
    void deleteRead(Transaction *t);
    void deleteWrite(Transaction *t);
};

#endif