import matplotlib.pyplot as plt
import os

# Data
rr = [0.5, 0.6, 0.7, 0.8, 0.9]
O2PL =[0.926, 0.74, 0.496, 0.262, 0.11]
OO2PL = []

# make a big plot
plt.figure(figsize=(16, 9))

# plot data
plt.plot(rr, O2PL, label='O2PL', marker='o', color='purple')
plt.plot(rr, OO2PL, label='optimistic-O2PL', marker='o', color='green')

# show grid lines
plt.grid(True)

plt.xlabel('read ratio')
plt.ylabel('Average aborts')
plt.title('Abort Count Analysis with Varying read ratio')
plt.legend()
script_name = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(f"{script_name}.png")
plt.show()