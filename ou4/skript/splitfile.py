import sys
import os

readfiles = []
try:
    for i in range(1, len(sys.argv)):
        readfiles.append(open(sys.argv[i], 'r'))
except:
    print("File(s) not found")
    sys.exit()

k = sys.argv[1].rfind('/')
if k != -1:
    resultfolder = sys.argv[1][:k]
else:
    resultfolder = "results"

try:
    os.mkdir(f"{resultfolder}/split")
except:
    pass
    
    
writefiles = []

for i in range(1, 6):
    writefiles.append(open(f'{resultfolder}/split/t{i}split.txt', 'w'))

for inp in readfiles:
    for line in inp:
        writefiles[int(line[0])-1].write(line[3:])
