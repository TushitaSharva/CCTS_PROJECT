#ifndef OPTIMISTICSCHEDULER_H
#define OPTIMISTICSCHEDULER_H

#include <vector>
#include <mutex>
#include <atomic>
#include <set>
#include <algorithm>
#include <thread>
#include "DataItem.h"
#include "WaitsForGraph.h"
#include "Transaction.h"

class OptimisticScheduler {
public:
    WaitsForGraph* G;
    std::mutex graphLock;
    std::vector<std::shared_ptr<DataItem>> shared;
    std::atomic<int> counter;
    // Set of committed transactions for Optimistic scheduler
    std::set<int> committedTransactions;
    std::mutex commitLock;

    OptimisticScheduler();
    ~OptimisticScheduler();
    void init(int m);
    Transaction* begin_trans(int threadID);
    bool read(Transaction* t, int index, int &localVal);
    bool write(Transaction* t, int index, int localVal);
    TransactionStatus tryCommit(Transaction* t);

    std::set<int> checkDependency(std::set<int>& dependencySet);
    void deleteAllWrites(Transaction* t);
};

#endif