import pandas as pd
import csv
import matplotlib.pyplot as plt

df = pd.read_csv('main.csv')
fig1 = plt.figure(figsize=(8, 8))
ax1 = fig1.add_subplot(111)

print (df)

n = df['n'].values

avg = (df['1'].values + df['2'].values + df['3'].values + df['4'].values + df['5'].values) / 5

acc = avg[0] / avg[1:]
eff = acc / n[1:]

# plot
plt.plot(n[1:],acc)
plt.plot(n[1:],eff)
plt.grid()
plt.legend(['acceleration', 'efficiency'])

plt.savefig("main.png", dpi=100)
