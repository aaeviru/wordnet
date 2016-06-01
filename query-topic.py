import re
import os
import sys
import numpy as np
from numpy import linalg as nplg
from scipy import linalg as sclg
from scipy.sparse import linalg
from scipy import sparse as sp

if len(sys.argv) != 2:
    print "input:topic folder\n"
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
aver1 = 0.0;
aver2 = 0.0;
aver3 = 0.0;
aver4 = 0.0;

a = np.load('/home/ec2-user/data/classinfo/vt.npy')
s = np.load('/home/ec2-user/data/classinfo/sigma.npy')
s = 1 / s
total = 0
for root, dirs, files in os.walk(sys.argv[1]):
        for name in files:
                filename = root + '/' + name
                if filename[len(filename)-1] == 't':#filename pattern
                        total = total + 1
                        fin = open(filename,"r")
                        lines = fin.readlines()
                        vec = np.zeros(623)
                        for line in lines:
                            term = line.strip('\n')
                            if term in wtol:
                                w = np.sqrt(len(wtol[term]))
                                tmp = np.zeros(623)
                                for j in wtol[term]:
                                    tmp = tmp + w * (s * a[:,j])
                                vec = vec + tmp
                        mmax = vec.max()
                        aver4 = aver4 + mmax / np.average(vec)
                        vec[vec.argmax()] = 0
                        mmax_2 = vec.max()
                        vec[vec.argmax()] = 0
                        mmax_3 = vec.max()
                        vec[vec.argmax()] = 0
                        mmax_4 = vec.max()
                        aver1 = aver1 + mmax / mmax_2
                        aver2 = aver2 + mmax_2 /mmax_3
                        aver3 = aver3 + mmax_3 /mmax_4
                                           
print "totalterm",total
print "aver1",aver1/total
print "aver2",aver2/total
print "aver3",aver3/total
print "aver4",aver4/total
