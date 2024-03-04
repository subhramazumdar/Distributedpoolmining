#!/bin/5h

#gcc create_graph.c -I/home/subhra/igraph/include -L/usr/local/lib -ligraph -lgfortran -llapack -lblas -lm -o create
 gcc cmm_test.c -I/home/subhra/igraph/include -ligraph -lm -fopenmp -o cmm

for((i=400000;i<=1000000;i*=2))
do
for((j=10;j<=50;j+=10))
do
for((m=10;m<=50;m+=20))
do
 ./cmm ../node$i\p$j/read_graph_edge.txt output_cmm.txt $m
done
done
done
