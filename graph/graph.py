import matplotlib.pyplot as plt
import glob
import os
import argparse


# File select index out of range exception
class ValueOutOfRange(Exception):
    def __str__(self):
        return 'Value out of range'


data_path = None

parser = argparse.ArgumentParser(description='Makes a graph of blablabla')
parser.add_argument('--data', '-d', nargs='?', action='store', dest='data_path', metavar='data file')
parser.add_argument('--version', '-v', action='version', version='v1.0')
args = parser.parse_args()

try:
    if args.data_path is None:
        data_path =  os.path.dirname(os.path.abspath(__file__)) + '/../data/*.TXT'
    else:
        sel = 0
        data_path = args.data_path

    # Find for data in data/ file
    data_files = glob.glob(data_path)

    # List found files
    if args.data_path is None:
        for i, fn in enumerate(data_files):
            print('[', i + 1, '] ', os.path.basename(fn))

        # Select file
        sel = None
        while sel not in range(len(data_files)) or type(sel) != int:
            try:
                sel = int(input('Select file > ')) - 1

                # Exit if entered value is 0
                if sel == -1:
                    exit()

                if sel not in range(len(data_files)):
                    raise ValueOutOfRange()
            except ValueOutOfRange as e:
                print(e)
            except ValueError as e:
                print(e)

    # Open data file
    try:
        f = open(data_files[sel], 'r')
    except Exception as e:
        print(e)

    # HYPERPARAMETERS 
    DATA_LINE = 3
    ZERO_LINE = 0

    # Parse data
    try:
        for i, line in enumerate(f):
            if i == DATA_LINE:
                data = line.split(':')
            if i == ZERO_LINE:
                zero_line = line.split(':')

        f.close()  # Close data file
    except Exception as e:
        print(e)

    f.close()  # Close data file

    # Parse tare value
    for item in zero_line:
        try:
            tare = int(item)
        except:
            pass

    print("Tare, zero value: ", tare)

    # Convert to integers and append to int_data list
    int_data = data
    for i, item in enumerate(data):
        try:
            int_data[i] = int(data[i])
        except:
            del int_data[i]

    del [int_data[len(int_data) - 1]]

    # Apply tare
    for i, d in enumerate(int_data):
        int_data[i] = abs(d) - abs(tare)  # With tare
        int_data[i] /= 20000  # *20000 to convert to Kh¡g and *9.8 for Newtopns

    plt.plot(range(len(int_data)), int_data, 'y')
    plt.grid()
    plt.xlabel("Lecture")
    plt.ylabel("Kgf")
    plt.show()
except KeyboardInterrupt as e:
    print("\nBye!")

