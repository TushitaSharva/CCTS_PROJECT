#include "OptimisticScheduler.h"
#include <iostream>
#include "Logger.h"

static Logger LOGGER;

OptimisticScheduler::OptimisticScheduler() : counter(1) {
    LOGGER.filenum = 2;
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

    LOGGER.OUTPUTT("    t", t->transactionId, " is trying to read from [", index, "]");
    std::unique_lock<std::mutex> lock(shared[index]->datalock);
    localVal = shared[index]->value;
    shared[index]->readList.insert(t->transactionId);
    LOGGER.OUTPUTT("    t", t->transactionId, " inserted in the readList of [", index, "]");
    for (auto it: shared[index]->writeList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
            LOGGER.OUTPUTT("    t", t->transactionId, " inserted ", it, " in the dependencySet");
        }
    }
    return true;
}

bool OptimisticScheduler::write(Transaction* t, int index, int localVal) {
    // local writes
    t->localWrites[index] = localVal;
    std::unique_lock<std::mutex> lock(shared[index]->datalock);
    shared[index]->writeList.insert(t->transactionId);
    LOGGER.OUTPUTT("    t", t->transactionId, " inserted in the writeList of [", index, "]");
    for (auto it: shared[index]->readList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
            LOGGER.OUTPUTT("    t", t->transactionId, " inserted ", it, " in the dependencySet");
        }
    }
    for (auto it: shared[index]->writeList) {
        if (t->transactionId != it) {
            t->dependencySet.insert(it);
            LOGGER.OUTPUTT("    t", t->transactionId, " inserted ", it, " in the dependencySet");
        }
    }
    return true;
}

TransactionStatus OptimisticScheduler::tryCommit(Transaction* t) {
    LOGGER.OUTPUTT("    t", t->transactionId, " is trying to commit");
    // Check if the transaction causes a cycle in the waits-for graph
    graphLock.lock();
    for (auto [index, value]: t->localWrites) {
        bool permission = G->addWriteOperation(t->transactionId, shared[index].get());
        if (!permission) {
            t->status = aborted;
            G->deleteNode(t->transactionId);
            deleteAllWrites(t);
            std::lock_guard<std::mutex> lock1(commitLock);
            committedTransactions.insert(t->transactionId);
            LOGGER.OUTPUTT("    t", t->transactionId, " aborted due to cycle in the WFG, added to committedTransactions");
            graphLock.unlock();
            return aborted;
        }
    }
    graphLock.unlock();

    LOGGER.OUTPUTT("    t", t->transactionId, " got permission to commit from WFG, starting the wait for dependencies");
    std::cout << "t" << t->transactionId << " has dependencies {";
    for(auto i : t->dependencySet) {
        std::cout << i << " ";
    }
    std::cout << "}\n";

    // Wait until all the dependencies are committed
    while(true) {
        std::set<int> remainingDependencies = checkDependency(t->dependencySet);
        if (remainingDependencies.empty()) {
            break;
        }
    }

    LOGGER.OUTPUTT("    t", t->transactionId, " has no remaining dependencies, proceeding to commit");
    
    for (auto [index, value]: t->localWrites) {
        std::unique_lock<std::mutex> lock(shared[index]->datalock);
        shared[index]->value = value;
        LOGGER.OUTPUTT("    t", t->transactionId, " wrote to [", index, "] a value ", value);
    }
    std::lock_guard<std::mutex> lock1(commitLock);
    committedTransactions.insert(t->transactionId);
    LOGGER.OUTPUTT("    t", t->transactionId, " committed successfully, added to committed transactions");
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