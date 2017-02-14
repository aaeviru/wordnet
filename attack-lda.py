import re
import os
import sys
import numpy as np
from numpy import linalg as nplg
from scipy import linalg as sclg
from scipy.sparse import linalg
from scipy import sparse as sp
from os import path

sys.path.append( path.dirname( path.dirname( path.abspath(__file__) ) ) )
from pythonlib import semantic as sm



if len(sys.argv) != 4:
    print "input: gb file,topic folder,otype\n"
    exit(0)

fwl = open("/home/ec2-user/git/statresult/wordlist-lda.txt","r")
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
fwl.close()

wtolu = sm.readwl("/home/ec2-user/git/statresult/wordslist_dsw_top1000.txt")


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

otype = int(sys.argv[3])
a = np.load('/home/ec2-user/git/statresult/lda-64-2000-top1000-phi.npy')
s = np.load('/home/ec2-user/git/statresult/lda-64-2000-top1000-pz.npy')

kk = a.shape[0]
user = 0
total = 0
hit = 0
notin = 0
totalterm = 0
maxnum = 0
maxnum_1 = 0
aver1 = 0.0
aver2 = 0.0
aver3 = 0.0
aver4 = 0.0


for root, dirs, files in os.walk(sys.argv[2]):
        for name in files:
                filename = root + '/' + name
                if filename[len(filename)-1] == 't':#filename pattern
                        fin = open(filename,"r")

                        lines = fin.readlines()
                        if len(lines) > 100:
                            continue
                        user = user + 1
                        r = []
                        d = []
                        for line in lines:
                            term = line.strip('\n')
                            if term in wtol:
                                for j in wtol[term]:
                                    r.append(itow[j])     

                            if term in bk:
                                for i in bk[term]:
                                    tmp = np.zeros(kk)
                                    if i in wtol:
                                        for j in wtol[i]:
                                            d.append(itow[j])
                        vec = sm.vecof3(r,a,s,wtolu,kk)
                        vec_t = sm.vecof3(d,a,s,wtolu,kk)
                        if vec.max() < 0.0000000000001:
                            print vec.max()
                            print filename 
                            continue
                        mmax = vec.argmax() 
                        if mmax == vec_t.argmax():
                            maxnum = maxnum + 1
                        else:
                            vec_t[vec_t.argmax()] = 0;
                            if mmax == vec_t.argmax():
                                maxnum_1 = maxnum_1 + 1
                        mmax_1 = vec.max()
                        aver4 = aver4 + mmax_1 / np.average(vec)
                        vec[vec.argmax()] = 0
                        mmax_2 = vec.max()
                        vec[vec.argmax()] = 0
                        mmax_3 = vec.max()
                        vec[vec.argmax()] = 0
                        mmax_4 = vec.max()
                        aver1 = aver1 + mmax_1 / mmax_2
                        aver2 = aver2 + mmax_2 / mmax_3
                        aver3 = aver3 + mmax_3 / mmax_4
                        
                        x = []#real query
                        y = []#attack result
                        for line in lines:
                            term = line.strip('\n')
                            total = total + 1
                            if term in bk:
                                check = -1
                                tt = ""
                                for i in bk[term]:
                                    tmp = 1 
                                    if i in wtol:
                                        totalterm = totalterm + 1
                                        for j in wtol[i]:
                                            tmp = tmp * a[:,j]
                                        tmp = tmp * s
                                        tmp = tmp/tmp.sum()
                                        if tmp[mmax] > check:
                                            check = tmp[mmax]
                                            tt = i
                                x.append(term)
                                y.append(tt)
                                if tt == term:
                                    hit = hit + 1
                            else:
                                notin = notin + 1
                        if otype == 1:
                            print "presion",1.0*hit/(total-notin)



print "total:",total
print "hit:",hit
print "notin:",notin
print "in",total-notin
print "presion",1.0*hit/(total-notin)
print "totalterm::",totalterm
print "per term:",1.0*totalterm/(total-notin)
total = 2908
print "maxnum:",maxnum
print "maxnum_1",maxnum_1
print "aver1",aver1/total
print "aver2",aver2/total
print "aver3",aver3/total
print "aver4",aver4/total
