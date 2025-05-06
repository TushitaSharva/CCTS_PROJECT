import matplotlib.pyplot as plt
import os
import numpy as np

# Data
numTransactions = [500, 1000, 1500, 2000, 2500]
O2PL = [0.522, 0.515, 0.500667, 0.469, 0.494]
OO2PL = []

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(numTransactions, O2PL, label='O2PL', marker='o', color='purple')
plt.plot(numTransactions, OO2PL, label='optimistic-O2PL', marker='o', color='green')

plt.yticks(np.arange(0, 1, 0.1)) 

# show grid lines
plt.grid(True)

plt.xlabel('total number of transactions')
plt.ylabel('Average aborts')
plt.title('Abort Count Analysis with Varying numTransactions')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()