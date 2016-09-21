import re
import os
import sys
import numpy as np
from numpy import linalg as nplg
from scipy import linalg as sclg
from scipy.sparse import linalg
from scipy import sparse as sp

if len(sys.argv) != 3:
    print "input: gb file,topic folder\n"
    exit(0)

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
fbk = open(sys.argv[1],"r")
for line in fbk:
    if len(line) < 2:
        for term in tmp:
            bk[term] = tmp
        tmp =[]
    else:
        tmp.append(itow[int(line.split()[0])])
fbk.close()

for root, dirs, files in os.walk(sys.argv[2]):
    for name in files:
        filename = root + '/' + name
        if filename[len(filename)-1] == 't':#filename pattern
            print "@" + filename
            num = 0
            fin = open(filename,"r")
            rquery = []
            lines = fin.readlines()
            for line in lines:
                term = line.strip('\n')
                if  term in bk:
                    rquery.append(term)
                    for i in bk[term]:
                        print i
                    print
            print "!" + " ".join(rquery)

