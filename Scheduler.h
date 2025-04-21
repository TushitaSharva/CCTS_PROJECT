#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <mutex>
#include <atomic>
#include "DataItem.h"
#include "WaitsForGraph.h"
#include "Transaction.h"

class Scheduler {
public:
    WaitsForGraph* G;
    std::mutex graphLock;
    std::vector<std::shared_ptr<DataItem>> shared;
    std::atomic<int> counter;

    Scheduler();
    ~Scheduler();
    void init(int m);
    Transaction* begin_trans(int threadID);
    bool read(Transaction* t, int index, int &localVal);
    bool write(Transaction* t, int index, int localVal);
    TransactionStatus tryCommit(Transaction* t);
};

#endif