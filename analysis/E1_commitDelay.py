import matplotlib.pyplot as plt
import os

# Data
numTransactions = [500, 1000, 1500, 2000, 2500]
O2PL = [2094.83, 7439.61, 18841.1, 35267.7, 53869.1]

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(numTransactions, O2PL, label='O2PL', marker='o', color='purple')

# show grid lines
plt.grid(True)

plt.xlabel('total number of transactions')
plt.ylabel('Average Commit delay')
plt.title('Commit Delay Analysis with Varying numTransactions')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()