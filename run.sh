#!/bin/sh
qrsh -N seq -pe smp 12 -q  canis6.q -nostdin ./wordnet >& result/seq.txt &
qrsh -N seq_n -pe smp 12 -q  canis5.q -nostdin ./wordnet result/synlink_n.txt result/sequence_n.txt >& result/seq_n.txt &
