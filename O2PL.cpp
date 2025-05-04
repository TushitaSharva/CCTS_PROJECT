#include <iostream>
#include <vector>
#include <random>
#include <string.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <mutex>
#include <time.h>
#include <chrono>
#include <fstream>
#include <atomic>
#include <mutex>
#include <thread>
#include <memory>
#include <algorithm>
#include "Logger.h"
#include "Transaction.h"
#include "DataItem.h"
#include "WaitsForGraph.h"
#include "Scheduler.h"

static Logger LOGGER;
int n, m, totalTrans, constVal, numIters;
float lambda;
float readRatio;
std::atomic<long long> totalCommitDelay{0};
std::atomic<long long> totalAborts{0};
std::shared_ptr<Scheduler> S;

void init(std::string filename) {
    std::ifstream inputfile(filename);
    inputfile >> n >> m >> totalTrans >> constVal >> lambda >> numIters >> readRatio;
    S->init(m);
    LOGGER.filenum = 1;
    inputfile.close();
    return;
}

float getRandomFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

double Timer(float exp_time) {
    static std::random_device rd;  // Non-deterministic random seed
    static std::mt19937 generate(rd());  // Mersenne Twister PRNG
    std::exponential_distribution<double> distr(1.0 / exp_time);
    return distr(generate);
}

void updtMem(int threadId) {
    TransactionStatus status = aborted;
    int abortCnt = 0;

    int numTrans = (totalTrans/n);

    if(threadId < totalTrans % n) { // For first reminder number of threads, just deal with one more transaction
        numTrans++;
    }

    for(int currTrans = 0; currTrans < numTrans; currTrans++) {
        abortCnt = 0;

        auto critStartTime = std::chrono::high_resolution_clock::now();

        do {
            Transaction *t = S->begin_trans(threadId);
            
            std::vector<int> perm(m);
            for(int i = 0; i < m; ++i) {
                perm[i] = i;
            }

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(perm.begin(), perm.end(), g);

            int localVal = 0;
            float readChance = getRandomFloat();
            bool readOnly = (readChance < readRatio ? true : false);
            
            for(int iter = 0; iter < std::min(numIters, m); iter++) {
                int randVal = rand()%constVal;
                
                bool readSuccess = S->read(t, perm[iter], localVal);
                if(!readSuccess) {
                    LOGGER.OUTPUTT("Thread id ", threadId, ", t", t->transactionId, " failed to read from [", perm[iter], "] a value ", localVal, ", breaking from the loop at time ");
                    break;
                }
                LOGGER.OUTPUTT("Thread id ", threadId, ", t", t->transactionId, " reads from [", perm[iter], "] a value ", localVal, " at time ");
                
                
                if(readOnly == false) {
                    localVal += randVal;
                    bool writeSuccess = S->write(t, perm[iter], localVal);
                    
                    if(!writeSuccess) {
                        LOGGER.OUTPUTT("Thread id ", threadId, ", t", t->transactionId, " failed to write to [", perm[iter], "] a value ", localVal, ", breaking from the loop at time ");
                        break;
                    }

                    LOGGER.OUTPUTT("Thread id ", threadId, ", t", t->transactionId, " writes to [", perm[iter], "] a value ", localVal, " at time ");
                }
                auto randTime = Timer(lambda);
                usleep((int)Timer(lambda) *1e3);
            }

            status = S->tryCommit(t);
            LOGGER.OUTPUTT(t->transactionId, "th transaction's try commit resulted in ", status);
            if(status == aborted) {
                abortCnt++;
            }
            delete t;
        } while (status != committed);

        auto critEndTime = std::chrono::high_resolution_clock::now();
        auto commitDelay = std::chrono::duration_cast<std::chrono::milliseconds>(critEndTime - critStartTime).count();

        LOGGER.OUTPUTT("Commit Delay: ", commitDelay, " milliseconds, ", "abortCount: ", abortCnt);
        totalCommitDelay.fetch_add(commitDelay);
        totalAborts.fetch_add(abortCnt);
    }
}

int main(int argc, char *argv[]) {
    S = std::make_shared<Scheduler>();
    init(argv[1]);
    auto start_time = std::chrono::high_resolution_clock::now();
    LOGGER.OUTPUTT(argv[1]);
    LOGGER.OUTPUTT("Started");

    std::thread threads[n];

    for(int i = 0; i < n; i++) {
        threads[i] = std::thread(updtMem, i);
    }

    for(int i = 0; i < n; i++) {
        threads[i].join();
    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    LOGGER.OUTPUTT("Stopped");

    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count();
    LOGGER.OUTPUTT("Total execution time: ", time_diff, " milliseconds");

    std::cout << "[O2PL] Avg commit delay = " << (double)totalCommitDelay.load()/totalTrans << " Avg aborts = " << (double)totalAborts.load()/totalTrans << "\n";

    return 0;
}