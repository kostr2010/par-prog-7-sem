import pandas as pd
import csv
import matplotlib.pyplot as plt

df = pd.read_csv('main.csv')
fig1 = plt.figure(figsize=(8, 8))
ax1 = fig1.add_subplot(111)

print (df)

n = df['n'].values

avg = (df['1'].values + df['2'].values) / 2

acc = avg[0] / avg
eff = acc / n

# plot
plt.plot(n,acc)
plt.plot(n,eff)
plt.grid()
plt.legend(['acceleration', 'efficiency'])

plt.savefig("main.png", dpi=100)


df = pd.read_csv('1g.csv')
fig1 = plt.figure(figsize=(8, 8))
ax1 = fig1.add_subplot(111)

print (df)

n = df['n'].values

avg = (df['1'].values + df['2'].values) / 2

acc = avg[0] / avg
eff = acc / n

# plot
plt.plot(n,acc)
plt.plot(n,eff)
plt.grid()
plt.legend(['acceleration', 'efficiency'])

plt.savefig("1g.png", dpi=100)
