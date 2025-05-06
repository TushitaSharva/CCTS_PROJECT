# AbortCount analysis with varying threads 2-32

import matplotlib.pyplot as plt
import os
import numpy as np

# Data
numThreads = [2, 4, 8, 16, 32]
O2PL =[0.016, 0.05, 0.09, 0.19, 0.418]
OO2PL = [0.038, 0.172, 0.23, 0.398, 0.43]

# make a big plot
plt.figure(figsize=(16, 9))


# plot data
plt.plot(numThreads, O2PL, label='O2PL', marker='o', color='purple')
plt.plot(numThreads, OO2PL, label='optimistic-O2PL', marker='o', color='green')
plt.yticks(np.arange(0, 1, 0.1)) 

# show grid lines
plt.grid(True)

plt.xlabel('number of threads')
plt.ylabel('Average aborts')
plt.title('Abort Count Analysis with Varying number of threads')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()