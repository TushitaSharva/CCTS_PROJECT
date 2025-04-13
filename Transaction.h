#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <atomic>

enum TransactionStatus {
    ongoing, aborted, committed
};

class Transaction
{
public:
    int transactionId;
    int threadId;
    std::atomic<TransactionStatus> status;

    Transaction(int trans_id, int thread_id) {
        transactionId = trans_id;
        threadId = thread_id;
        status.store(ongoing);
    }
};

#endif