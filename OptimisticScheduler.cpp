#include "Scheduler.h"

Scheduler::Scheduler() : counter(1) {
    G = new WaitsForGraph();
}

Scheduler::~Scheduler() {
    delete G;
    shared.clear();
}

void Scheduler::init(int m) {
    shared.resize(m);
    for (int i=0; i<m; i++) {
        shared[i] = std::make_shared<DataItem>();
    }
}

Transaction* Scheduler::begin_trans(int threadID) {
    int txnID = counter.fetch_add(1);
    Transaction* t = new Transaction(txnID, threadID);
    return t;
}

bool Scheduler::read(Transaction* t, int index, int &localVal) {
    {
        std::lock_guard<std::mutex> lock(graphLock);
        bool permission = G->addReadOperation(t->transactionId, shared[index].get());
        if (!permission) {
            t->status = aborted;        
            return false;
        }
    }
    
    Node* node = shared[index]->addRead(t);
    {
        std::unique_lock<std::mutex> lock(node->mtx);
        node->cv.wait(lock, [&]() {return node->isAtHead; });
        localVal = shared[index]->value;
    }
    shared[index]->deleteRead(t);
    return true;
}

bool Scheduler::write(Transaction* t, int index, int localVal) {
    {
        std::lock_guard<std::mutex> lock(graphLock);
        bool permission = G->addWriteOperation(t->transactionId, shared[index].get());
        if (!permission) {
            t->status = aborted;
            return false;
        }
    }

    Node* node = shared[index]->addWrite(t);
    {
        std::unique_lock<std::mutex> lock(node->mtx);
        node->cv.wait(lock, [&]() {return node->isAtHead; });
        shared[index]->value = localVal;
    }
    
    shared[index]->deleteWrite(t);
    return true;
}

TransactionStatus Scheduler::tryCommit(Transaction* t) {
    if (t->status == aborted) return aborted;
    return committed; 
}

