import matplotlib.pyplot as plt
import glob

print("lolaso")

## Find for data in data/ file
data_files = glob.glob('data/*')
## List found files
for i, fn in enumerate(data_files):
    print('[',i+1,'] ', fn)

## Select file
sel = None  
while sel not in range(len(data_files)) and type(sel) != int:
    try: 
        sel = int(input('Select file >')) - 1
    except:
        pass

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

## Apply tare
for i, d in enumerate(int_data):
    int_data[i] = abs(d)-abs(tare) # With tare
    int_data[i] /= 20000 # *20000 to convert to Kh¡g and *9.8 for Newtopns

plt.plot(range(len(int_data)), int_data, 'y')
plt.grid()
plt.xlabel("Lecture")
plt.ylabel("Kgf")
plt.show()
