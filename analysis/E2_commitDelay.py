# AbortCount analysis with varying variables 1000-5000

import matplotlib.pyplot as plt
import os

# Data
variables = [1000, 2000, 3000, 4000, 5000]
O2PL = [1796.42, 855.366, 670.534, 607.568, 521.158]
OO2PL = [2694.56, 1478.1, 1290.2, 1148.55, 970.73]

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(variables, O2PL, label='O2PL', marker='o', color='purple')
plt.plot(variables, OO2PL, label='optimistic-O2PL', marker='o', color='green')

# show grid lines
plt.grid(True)

plt.xlabel('number of data items')
plt.ylabel('Average Commit Delay')
plt.title('Commit Delay Analysis with Varying number of data items')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()