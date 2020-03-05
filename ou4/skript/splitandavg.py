import sys
import os

if len(sys.argv) < 2:
    print("Usage(without brackets): python sortfiles.py {your results} (can take several results)\n")
    sys.exit()

readfiles = []

try:
    for i in range(1, len(sys.argv)):
        readfiles.append(open(sys.argv[i], 'r'))
except:
    print("File(s) not found")
    sys.exit()

tests = [[], [], [], [], []]

for readfile in readfiles:
    for line in readfile:
        tests[int(line[0]) - 1].append(line[3:])

avgtest = [{}, {}, {}, {}, {}]

for test in tests:
    for run in test:
        run = run.split(",")
        run = [int(run[0]), int(run[1])]
        if run[0] not in avgtest[tests.index(test)]:
            avgtest[tests.index(test)][run[0]] = [run[1]]
        else:
            avgtest[tests.index(test)][run[0]].append(run[1])


k = sys.argv[1].rfind('/')
if k != -1:
    resultfolder = sys.argv[1][:k]
else:
    resultfolder = "results"

try:
    os.mkdir(f"{resultfolder}/splitavg")
except:
    pass

writefiles = []
for i in range(1,6):
	writefiles.append(open(f"{resultfolder}/splitavg/t{i}avg.txt", 'w'))

x = 0
for dic in avgtest:
    for item in dic:
        dic[item] = round(sum(dic[item]) / len(dic[item]))
        writefiles[x].writelines(f"{item}, {dic[item]}\n")
    x = x + 1
