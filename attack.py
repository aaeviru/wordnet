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
from pythonlib import getapy as gp


if len(sys.argv) != 4:
    print "input: gb file,topic folder,otype\n"
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

otype = int(sys.argv[3])
a = np.load('/home/ec2-user/data/classinfo/vt.npy')
s = np.load('/home/ec2-user/data/classinfo/sigma.npy')
kk = 623
user = 0
s = 1 / s
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
srp20 = 0.0
srp100 = 0.0
srp500 = 0.0
srp1000 = 0.0

if otype == 2:
    wam = gp.init("NTCIR")
    getar = gp.intp(1000)

for root, dirs, files in os.walk(sys.argv[2]):
        for name in files:
                filename = root + '/' + name
                if filename[len(filename)-1] == 't':#filename pattern
                        fin = open(filename,"r")

                        lines = fin.readlines()
                        if len(lines) > 100:
                            continue
                        user = user + 1
                        if otype == 1:
                            print filename,user
                        vec = np.zeros(kk)
                        vec_t = np.zeros(kk)
                        for line in lines:
                            term = line.strip('\n')
                            if term in wtol:
                                w = np.sqrt(len(wtol[term]))
                                tmp = np.zeros(kk)
                                for j in wtol[term]:
                                    tmp = tmp + w * (s * a[:,j])
                                vec_t = vec_t + tmp

                            if term in bk:
                                for i in bk[term]:
                                    tmp = np.zeros(kk)
                                    if i in wtol:
                                        w = np.sqrt(len(wtol[i]))
                                        for j in wtol[i]:
                                            tmp = tmp + abs(w * (s * a[:,j]))
                                    vec = vec + tmp
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
                                    tmp = np.zeros(kk)
                                    if i in wtol:
                                        totalterm = totalterm + 1
                                        w = np.sqrt(len(wtol[i]))
                                        for j in wtol[i]:
                                            tmp = tmp + w * (s * a[:,j])
                                    if tmp[mmax] > check:
                                        check = tmp[mmax]
                                        tt = i
                                x.append(term)
                                y.append(tt)
                                if tt == term:
                                    hit = hit + 1
                            else:
                                notin = notin + 1
                        if otype == 2:
                            srlen = gp.search(wam,x,getar,1000)
                            rqn = np.array([getar[i] for i in range(srlen)])
                            srlen = gp.search(wam,y,getar,1000)
                            dqn = np.array([getar[i] for i in range(srlen)])
                            srp20 = srp20 + len(np.intersect1d(rqn[0:20],dqn[0:20]))*1.0/20.0
                            srp100 = srp100 + len(np.intersect1d(rqn[0:100],dqn[0:100]))*1.0/100.0
                            srp500 = srp500 + len(np.intersect1d(rqn[0:500],dqn[0:500]))*1.0/500.0
                            srp1000 = srp1000 + len(np.intersect1d(rqn,dqn))*1.0/len(rqn)
                        if otype == 1:
                            print "presion",1.0*hit/(total-notin)
                            print 'srp20:',srp20*1.0/user
                            print 'srp100:',srp100*1.0/user
                            print 'srp500:',srp500*1.0/user
                            print 'srp1000:',srp1000*1.0/user



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
if otype == 2:
    print 'srp20/(total+usernum):',srp20*1.0/(total)
    print 'srp100/(total+usernum):',srp100*1.0/(total)
    print 'srp500/(total+usernum):',srp500*1.0/(total)
    print 'srp1000/(total+usernum):',srp1000*1.0/(total)

