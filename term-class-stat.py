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

a = np.load('/home/ec2-user/data/classinfo/vt.npy')

total = 0
hit = 0
notin = 0
totalterm = 0

for root, dirs, files in os.walk(sys.argv[2]):
        for name in files:
                filename = root + '/' + name
                if filename[len(filename)-1] == 't':#filename pattern
                        vec = np.zeros(623)
                        print filename
                        fin = open(filename,"r")

                        lines = fin.readlines()
                        for line in lines:
                            tmp = np.zeros(623)
                            term = line.strip('\n')
                            if term in wtol:
                                w = np.sqrt(len(wtol[term]))
                                for j in wtol[term]:
                                    tmp = tmp + w * a[:,j]
                                vec = vec + tmp
                                mmax = tmp.argmax()
                                print term
                                print mmax,tmp[mmax]
                                tmp[mmax] = 0
                                mmax = tmp.argmax()
                                print mmax,tmp[mmax]
                        print
                        mmax = vec.argmax()
                        print mmax,vec[mmax]
                        vec[mmax] = 0
                        mmax = vec.argmax()
                        print mmax,vec[mmax]

                        print
