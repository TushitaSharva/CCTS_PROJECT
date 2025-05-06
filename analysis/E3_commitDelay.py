# AbortCount analysis with varying threads 2-32

import matplotlib.pyplot as plt
import os

# Data
numThreads = [2, 4, 8, 16, 32]
O2PL =[452.5, 481.994, 531.406, 816.576, 1609.22]
OO2PL = []

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(numThreads, O2PL, label='O2PL', marker='o', color='purple')
plt.plot(numThreads, OO2PL, label='optimistic-O2PL', marker='o', color='green')

# show grid lines
plt.grid(True)

plt.xlabel('number of threads')
plt.ylabel('Average commit delay')
plt.title('Commit delay Analysis with Varying number of threads')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()