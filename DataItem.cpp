#include "DataItem.h"

Node* DataItem::addRead(Transaction *t) {
    this->datalock.lock();
    Node* rnode = this->nodeList->addReadNode(t);
    this->readList.insert(t->transactionId);
    this->datalock.unlock();
    return rnode;
}

Node* DataItem::addWrite(Transaction *t) {
    this->datalock.lock();
    Node *wnode = this->nodeList->addWriteNode(t);
    this->writeList.insert(t->transactionId);
    this->datalock.unlock();
    return wnode;
}

void DataItem::deleteRead(Transaction *t) {
    this->datalock.lock();
    this->nodeList->deleteReadNode(t);
    this->datalock.unlock();
}

void DataItem::deleteWrite(Transaction *t) {
    this->datalock.lock();
    this->nodeList->deleteWriteNode(t);
    this->datalock.unlock();
}
