import matplotlib.pyplot as plt
import glob

## Find for data in data/ file
data_files = glob.glob('data/*')

## List found files
for i, fn in enumerate(data_files):
    print('[',i+1,'] ', fn)

## Select file
sel = None  
while sel not in range(len(data_files)) and type(sel) != int:
    sel = int(input('Select file >')) - 1

## Open data file
f = open(data_files[sel], 'r')

## HYPERPARAMETERS 
DATA_LINE = 3
ZERO_LINE = 0

## Parse data
for i, line in enumerate(f):
    if i == DATA_LINE:
        data = line.split(':')
    if i == ZERO_LINE:
        zero_line = line.split(':')

f.close() ## Close data file

## Parse tare value
for item in zero_line:
    try:
        tare = int(item)
    except:
        pass

print("Tare, zero value: ", tare)

## Convert to integers and append to int_data list
int_data = data
for i, item in enumerate(data):
    try:
        int_data[i] = int(data[i])
    except:
        del int_data[i]

del[int_data[len(int_data)-1]]

#####################################

# plt.plot(range(len(int_data)), int_data, 'y')
# plt.bar(range(len(int_data)), int_data)

## Offset
#offseted_data = int_data
#for i, val in enumerate(int_data):
#    if val <= tare:
#        offseted_data[i] = 0        
#
#int_data = offseted_data
#####################################

for i, d in enumerate(int_data):
    int_data[i] = abs(d)

plt.plot(range(len(int_data)), int_data, 'y')

from scipy.signal import lfilter

## 80Hz
n = 27  # the larger n is, the smoother curve will be
## 10Hz
# n = 5

b = [1.0 / n] * n
a = 1

yy = lfilter(b,a, int_data)
yy = abs(yy)

plt.plot(range(len(int_data)), yy, 
         linewidth=2, linestyle="-", c="b")  # smooth by filter


### PLOT DATA ###
#plt.plot(range(len(int_data)), offseted_data, 'r')
plt.xlabel("Lecture")
plt.ylabel("Sensor value")
plt.show()
