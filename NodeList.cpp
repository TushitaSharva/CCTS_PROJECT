#include "NodeList.h"

NodeList::NodeList() {
    this->head = nullptr;
    this->tail = nullptr;
}

Node* NodeList::addReadNode(Transaction *t) {
    std::lock_guard<std::mutex> lock(listMutex);
    ReadNode* newNode;
    if (!head) {
        newNode = new ReadNode();
        newNode->tlist.insert(t);
        head = tail = newNode;
        newNode->isAtHead = true;
        newNode->cv.notify_all();
        return newNode;
    } else if (head == tail && head->type == READ) {
        ReadNode* rnode = dynamic_cast<ReadNode*>(head);
        rnode->tlist.insert(t);
        head->cv.notify_all();
    } else {
        ReadNode* rnode = dynamic_cast<ReadNode*>(tail);
        if (rnode) {
            rnode->tlist.insert(t);
            return rnode;
        } else {
            newNode = new ReadNode();
            newNode->tlist.insert(t);
            tail->next = newNode;
            tail = newNode;
        }
    }
    return newNode;
}

Node* NodeList::addWriteNode(Transaction *t) {
    std::lock_guard<std::mutex> lock(listMutex);
    WriteNode* newNode = new WriteNode(t);
    if (!head) {
        head = tail = newNode;
        newNode->isAtHead = true;
        newNode->cv.notify_all();
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    return newNode;
}

void NodeList::deleteReadNode(Transaction *t) {
    std::lock_guard<std::mutex> lock(listMutex);
    ReadNode* rnode = dynamic_cast<ReadNode*>(head);
    if (!rnode || rnode->tlist.find(t) == rnode->tlist.end()) return;

    rnode->tlist.erase(t);
    if (rnode->tlist.size() == 0) {
        Node* toDel = head;
        head = head->next;
        if (head) {
            head->isAtHead = true;
            head->cv.notify_all();
        } else {
            tail = nullptr;
        }
        delete toDel;
    }
}

void NodeList::deleteWriteNode(Transaction *t) {
    std::lock_guard<std::mutex> lock(listMutex);
    WriteNode* wnode = dynamic_cast<WriteNode*>(head);
    if (!wnode || wnode->transaction != t) return;

    Node* toDel = head;
    head = head->next;
    if (head) {
        head->isAtHead = true;
        head->cv.notify_all();
    } else {
        tail = nullptr;
    }
    delete toDel; 
}

NodeList::~NodeList() {
    Node* curr = head;
    while(curr) {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }
    head = nullptr;
    tail = nullptr;
}