import os
from matplotlib import pyplot

abs_path = os.path.dirname(os.path.abspath(__file__))

data_file = '\out.txt'

print(abs_path)

datas = []
x = []
data = []
i = 0
with open(abs_path+data_file, 'r', encoding='utf-8') as f:
    rs = f.readlines()

for r in rs:
    ds = r.split(' ')
    if (len(ds) > 1):
        datas.append([])
        for d in ds:
            if '.' in d:
                datas[i].append(float(d))
        i = i+1

for d in datas:
    pyplot.plot(d)
    
pyplot.grid(True)
pyplot.show()
pass
