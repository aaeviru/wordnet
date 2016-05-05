# wordnet
main.cpp:create term-sequence
gb.cpp:create term-basket from word-sequence
wordnet.cpp:functions about wordnet,sqlite
wnjpn.1.1.db,wnjpn.db:Japanese Wordnet
init.cpp:calculate term-specificit
attack.py:attack wordnet-query with term-class-lsa
log:
	wordsp.txt:term-specificit
result:
	synset.txt
	sequence.txt:term-sequence
	sequence_n.txt
	sequence_jp.txt
	sequence_jp_n.txt
	synlink.txt:number of relationships of synets
	synlink_n.txt
	synlink_jp.txt
	synlink_jp_n.txt
	gb.txt:term-basket
	
Makefile:make,clean
run.sh:ex to run
