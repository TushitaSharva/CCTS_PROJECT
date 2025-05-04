# AbortCount analysis with varying variables 1000-5000

import matplotlib.pyplot as plt
import os

# Data
variables = [1000, 2000, 3000, 4000, 5000]
O2PL = [0.452, 0.17, 0.062, 0.048, 0.022]

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(variables, O2PL, label='O2PL', marker='o', color='purple')

# show grid lines
plt.grid(True)

plt.xlabel('number of data items')
plt.ylabel('Average aborts')
plt.title('Abort Count Analysis with Varying number of data items')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()