obj = wordnet.o main.o
CC = g++
CFLAGS = -l sqlite3 

all:wordnet init gb wordlist
wordnet:$(obj)
	g++ -o wordnet $(obj) -l sqlite3
init:wordnet.o init.o
	g++ -o init wordnet.o init.o -l sqlite3
gb:wordnet.o gb.o
	 g++ -o gb wordnet.o gb.o -l sqlite3
wordlist:wordlist.o wordnet.o
	$(CC) $(CFLAGS) $^ -o $@
.PHONY : clean
clean :
	-rm wordnet $(obj) init init.o gb gb.o

