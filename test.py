import random
fout = open("/home/ko/wordnet/result/ranktest.txt","w")
fin = open("/home/ko/wordnet/result/sequence_n1.txt","r")

for line in fin:
	fout.write(line.strip('\n')+" "+str(random.randint(1,10))+"\n")
fout.close()
fin.close()
