#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <atomic>
#include <set>
#include <map>

enum TransactionStatus {
    ongoing, aborted, committed
};

class Transaction
{
public:
    int transactionId;
    int threadId;
    std::atomic<TransactionStatus> status;
    // dependency set and local writes map for Optimistic scheduler
    std::set<int> dependencySet;
    std::map<int, int> localWrites;
    std::set<int> readlist;

    Transaction(int trans_id, int thread_id) {
        transactionId = trans_id;
        threadId = thread_id;
        status.store(ongoing);
    }

    ~Transaction() = default;
};

#endif