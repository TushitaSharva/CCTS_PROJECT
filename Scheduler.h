#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <set>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include "DataItem.h"
#include "WaitsForGraph.h"

class Scheduler {
public:
    WaitsForGraph G;
    std::mutex graphlock;
    std::vector<std::shared_ptr<DataItem>> shared;

    bool read(Transaction *t, int index, int *localVal) {
        graphlock.lock();
        bool permission = G.addReadOperation(t->transactionId, shared[index].get(), READ);
        if(!permission) {
            t->status = aborted;
            graphlock.unlock();
            return false;
        }
        graphlock.unlock();
    
        Node* node = shared[index]->addRead(t);
        std::unique_lock<std::mutex> lock(node->mtx);
        node->cv.wait(lock, [&]() {return node->isAtHead; });
        *localVal = shared[index]->value;
    
        shared[index]->deleteRead(t);
        return true;
    }

    bool write(Transaction *t, int index, int localVal) {
        graphlock.lock();
        bool permission = G.addWriteOperation(t->transactionId, shared[index].get(), WRITE);
        if(!permission) {
            t->status = aborted;
            graphlock.unlock();
            return false;
        }
        graphlock.unlock();
    
        Node* node = shared[index]->addWrite(t);
        std::unique_lock<std::mutex> lock(node->mtx);
        node->cv.wait(lock, [&]() {return node->isAtHead; });
        shared[index]->value = localVal;
        shared[index]->deleteWrite(t);
        return true;
    }

    TransactionStatus tryCommit(Transaction *t) {
        if(t->status == aborted) {
            return aborted;
        }
    
        return committed;
    }
};

#endif