#include "NodeList.h"
#include "Logger.h"

static Logger LOGGER;

NodeList::NodeList() {
    this->head = nullptr;
    this->tail = nullptr;
}

Node* NodeList::addReadNode(Transaction *t) {
    ReadNode* newNode;
    if (!head) {
        LOGGER.OUTPUTT("Creating a READ node to an empty list, transation ", t->transactionId);
        newNode = new ReadNode();
        newNode->tlist.insert(t);
        head = tail = newNode;
        newNode->isAtHead = true;
        newNode->cv.notify_all();
        return newNode;
    } else if (head == tail && head->type == READ) {
        LOGGER.OUTPUTT("Inserting a READ node to existing head node, transation ", t->transactionId);
        ReadNode* rnode = dynamic_cast<ReadNode*>(head);
        rnode->tlist.insert(t);
        head->cv.notify_all();
        return rnode;
    } else {
        ReadNode* rnode = dynamic_cast<ReadNode*>(tail);
        if (rnode) {
            LOGGER.OUTPUTT("Inserting a READ node to existing tail node, transation ", t->transactionId);
            rnode->tlist.insert(t);
            return rnode;
        } else {
            LOGGER.OUTPUTT("Creating a READ node next to a tail node, transation ", t->transactionId);
            newNode = new ReadNode();
            newNode->tlist.insert(t);
            tail->next = newNode;
            tail = newNode;
        }
    }
    return newNode;
}

Node* NodeList::addWriteNode(Transaction *t) {
    WriteNode* newNode = new WriteNode(t);
    if (!head) {
        LOGGER.OUTPUTT("Creating a WRITE node to an empty list ", t->transactionId);
        head = tail = newNode;
        newNode->isAtHead = true;
        newNode->cv.notify_all();
    } else {
        LOGGER.OUTPUTT("Creating a WRITE node next to a tail node ", t->transactionId);
        tail->next = newNode;
        tail = newNode;
    }
    return newNode;
}

void NodeList::deleteReadNode(Transaction *t) {
    ReadNode* rnode = dynamic_cast<ReadNode*>(head);
    if (!rnode || rnode->tlist.find(t) == rnode->tlist.end()) return;
    
    rnode->tlist.erase(t);
    LOGGER.OUTPUTT("Erasing the transaction ", t->transactionId, " from the wait list of the READ node");
    if (rnode->tlist.size() == 0) {
        LOGGER.OUTPUTT("Deleting the READ node, all transactions were processed");
        Node* toDel = head;
        head = head->next;
        if (head) {
            LOGGER.OUTPUTT("Signalling the next node after deleting the READ node");
            head->isAtHead = true;
            head->cv.notify_all();
        } else {
            LOGGER.OUTPUTT("No next node to signal after deleting the READ node");
            tail = nullptr;
        }
        delete toDel;
    }
}

void NodeList::deleteWriteNode(Transaction *t) {
    WriteNode* wnode = dynamic_cast<WriteNode*>(head);
    if (!wnode || wnode->transaction != t) return;
    LOGGER.OUTPUTT("Deleting the WRITE Node of transaction ", t->transactionId);
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