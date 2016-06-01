obj = wordnet.o main.o
CC = g++
CFLAGS = -l sqlite3 

all:wordnet init gb wordlist synset synlink txttodb newgb
wordnet:$(obj)
	g++ -o wordnet $(obj) -l sqlite3
init:wordnet.o init.o
	g++ -o init wordnet.o init.o -l sqlite3
gb:wordnet.o gb.o
	 g++ -o gb wordnet.o gb.o -l sqlite3
wordlist:wordlist.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@
synset:synset.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@
synlink:synlink.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@
txttodb:txttodb.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@
newgb:newgb.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean :
	-rm wordnet $(obj) init init.o gb gb.o synset synset.o synlink synlink.o newgb newgb.o txttodb txttodb.o

