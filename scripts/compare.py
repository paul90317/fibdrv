import numpy as np
import matplotlib.pylab as plt
import os

def outlier_filter(datas, threshold = 2):
    datas = np.array(datas)
    z = np.abs((datas - datas.mean()) / datas.std())
    return datas[z < threshold]

datas :list[dict[int,list[int]]] = []

max_term = 1000
times = 50
modes = ['dynamic programing', 'fast doubling']
goods = ['+', '*']
for mode in range(len(modes)):
    datas.append({})
    data = datas[-1]
    for li in os.popen(f'sudo ./clients/fibget {max_term} {times} {mode}').readlines():
        term, ktime, _, _ = li.split(' ')
        term = int(term)
        ktime = int(ktime)
        if term not in data:
            data[term] = [ktime]
        else:
            data[term].append(ktime)

x :list[int] = []
for term, numbers in data.items():
    x.append(term)

fig, ax = plt.subplots(1, 1, sharey = True)
ax.set_title('perf', fontsize = 16)
ax.set_xlabel(r'$n_{th}$ fibonacci', fontsize = 16)
ax.set_ylabel('time (ns)', fontsize = 16)

for mode in range(len(modes)):
    y1 :list[int] = []
    for term, numbers in datas[mode].items():
        y1.append(np.mean(outlier_filter(numbers, 2)))
    ax.plot(x, y1, marker = goods[mode], markersize = 7, label = modes[mode])

ax.legend(loc = 'upper left')

plt.show()

