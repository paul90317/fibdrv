import numpy as np
import matplotlib.pylab as plt
import os

def outlier_filter(datas, threshold = 2):
    datas = np.array(datas)
    z = np.abs((datas - datas.mean()) / datas.std())
    return datas[z < threshold]

data :dict[int,tuple[list[int],list[int]]] = {}

max_term = 1000
times = 50
mode = 1
for li in os.popen(f'sudo ./clients/fibget {max_term} {times} {mode}').readlines():
    term, ktime, utime, _ = li.split(' ')
    term = int(term)
    ktime = int(ktime)
    utime = int(utime)
    if term not in data:
        data[term] = ([ktime],[utime])
    else:
        data[term][0].append(ktime)
        data[term][1].append(utime)

x :list[int] = []
y1 :list[int] = []
y2 :list[int] = []
y3 :list[int] = []

for i, d in data.items():
    x.append(i)
    y1.append(np.mean(outlier_filter(d[0], 2)))
    y2.append(np.mean(outlier_filter(d[1], 2)))
    y3.append(y2[-1] - y1[-1])

fig, ax = plt.subplots(1, 1, sharey = True)
ax.set_title('perf', fontsize = 16)
ax.set_xlabel(r'$n_{th}$ fibonacci', fontsize = 16)
ax.set_ylabel('time (ns)', fontsize = 16)

ax.plot(x, y1, marker = '+', markersize = 7, label = 'kernel')
ax.plot(x, y2, marker = '*', markersize = 3, label = 'user')
ax.plot(x, y3, marker = '^', markersize = 3, label = 'kernel to user')
ax.legend(loc = 'upper left')

plt.show()

