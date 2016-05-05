import numpy as np
import sys

fwl = open("/home/ec2-user/git/wordnet/result/wordlist.txt","r")
wtol = {}
itow = {}
prev = -1
for line in fwl:
    tmp = line.split()
    if len(tmp) == 3:
        i = int(tmp[0])
        w = tmp[2]
        l = int(tmp[1])
        if i == prev:
            wtol[w].append(l)
        else:
            wtol[w] = [l]
            itow[i] = w
        prev = i
    elif len(tmp) == 2:
        itow[int(tmp[0])] = tmp[1]
    else:
        print line
        raw_input()

fwl.close()

tmp = []
bk = {}
fbk = open("/home/ec2-user/git/wordnet/result/gb.txt","r")
for line in fbk:
    if len(line) < 2:
        for term in tmp:
            bk[term] = tmp
        tmp =[]
    else:
        tmp.append(itow[int(line.split()[0])])
fbk.close()


fin = open(sys.argv[1],"r")
lines = fin.readlines()
for line in lines:
    term = line.strip('\n')
    if term in bk:
        for i in bk[term]:
            if i in wtol:
                print i
            else:
                print i,0
    print " "

