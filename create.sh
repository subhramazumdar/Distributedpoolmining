#!/bin/5h

#gcc create_graph.c -I/home/subhra/igraph/include -L/usr/local/lib -ligraph -lgfortran -llapack -lblas -lm -o create
 gcc distributed_greedy.c -I/home/subhra/igraph/include -ligraph -lm -fopenmp -o greedy

for((i=50000;i<=1000000;i*=2))
do
for((j=10;j<=50;j+=10))
do
 ./greedy ../node$i\p$j/read_graph_edge.txt output_distributed.txt
done
done

