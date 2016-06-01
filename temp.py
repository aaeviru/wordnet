fin = open("result/sequence_all_test.txt","r")
fout = open("result/sequence_all_test1.txt","w")
num = 0
print len("\n")
for line in fin:
	num = num + 1
	if len(line) < 2:
            break 
	fout.write(line)
print num
fin.close()
fout.close()
