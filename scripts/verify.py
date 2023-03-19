#!/usr/bin/env python3

result = []
result_split = []
dics = []
expect = [0, 1]
def F(i):
    if i < len(expect):
        return expect[i]
    for i in range(len(expect), i + 1):
        expect.append(expect[-1] + expect[-2])
    return expect[-1]

with open('out', 'r') as f:
    result=f.readlines()
    
for r in result:
    if (r.find('Reading') != -1):
        result_split.append(r.split(' '))
        k = int(result_split[-1][5].split(',')[0])
        f0 = int(result_split[-1][9].split('.')[0])
        dics.append((k, f0))

for i in dics:
    fib = i[1]
    if (F(i[0]) != fib):
        print('f(%s) fail' % str(i[0]))
        print('input:    %s' %(fib))
        print('expected: %s' %(F(i[0])))
        exit("wrong answer!")
