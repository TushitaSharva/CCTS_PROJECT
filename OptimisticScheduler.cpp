#include "OptimisticScheduler.h"
#include <iostream>

OptimisticScheduler::OptimisticScheduler() : counter(1) {
    G = new WaitsForGraph();
}

OptimisticScheduler::~OptimisticScheduler() {
    delete G;
    shared.clear();
}

void OptimisticScheduler::init(int m) {
    shared.resize(m);
    for (int i=0; i<m; i++) {
        shared[i] = std::make_shared<DataItem>();
    }
}

Transaction* OptimisticScheduler::begin_trans(int threadID) {
    int txnID = counter.fetch_add(1);
    Transaction* t = new Transaction(txnID, threadID);
    return t;
}

bool OptimisticScheduler::read(Transaction* t, int index, int &localVal) {

    std::unique_lock<std::mutex> lock(shared[index]->datalock);
    localVal = shared[index]->value;
    shared[index]->readList.insert(t->transactionId);
    for (auto it: shared[index]->writeList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
        }
    }
    return true;
}

bool OptimisticScheduler::write(Transaction* t, int index, int localVal) {
    // local writes
    t->localWrites[index] = localVal;
    std::unique_lock<std::mutex> lock(shared[index]->datalock);
    for (auto it: shared[index]->readList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
        }
    }
    for (auto it: shared[index]->writeList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
        }
    }
    shared[index]->writeList.insert(t->transactionId);
    return true;
}

TransactionStatus OptimisticScheduler::tryCommit(Transaction* t) {
    // Check if the transaction causes a cycle in the waits-for graph
    std::unique_lock<std::mutex> lock(graphLock);
    for (auto [index, value]: t->localWrites) {
        bool permission = G->addWriteOperation(t->transactionId, shared[index].get());
        if (!permission) {
            t->status = aborted;
            G->deleteNode(t->transactionId);
            deleteAllWrites(t);
            std::lock_guard<std::mutex> lock1(commitLock);
            committedTransactions.insert(t->transactionId);
            return aborted;
        }
    }

    std::cout << t->transactionId << std::endl;

    // Wait until all the dependencies are committed
    while(true) {
        std::set<int> remainingDependencies = checkDependency(t->dependencySet);
        if (remainingDependencies.empty()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << t->transactionId << std::endl;

    for (auto [index, value]: t->localWrites) {
        std::unique_lock<std::mutex> lock(shared[index]->datalock);
        shared[index]->value = value;
    }
    std::lock_guard<std::mutex> lock1(commitLock);
    committedTransactions.insert(t->transactionId);
    return committed;
}

std::set<int> OptimisticScheduler::checkDependency(std::set<int>& dependencySet) {
    std::unique_lock<std::mutex> lock(commitLock);
    std::set<int> result;
    std::set_difference(
        dependencySet.begin(), dependencySet.end(),
        committedTransactions.begin(), committedTransactions.end(),
        std::inserter(result, result.begin())
    );
    return result;
}

void OptimisticScheduler::deleteAllWrites(Transaction* t) {
    for (auto [index, value]: t->localWrites) {
        std::unique_lock<std::mutex> lock(shared[index]->datalock);
        shared[index]->writeList.erase(t->transactionId);
    }
}