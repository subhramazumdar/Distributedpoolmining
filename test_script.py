import os
import sys
n=50000
while n<=4000000:
    p=[10,50,100,200,300]
    for i in p:
    	k="data/graph-"+str(n)+"-"+str(i)+".edgelist"
    	print(k)
    	print(i)
    	os.system("python3 create_graph.py "+str(n)+" "+str(i)+" "+str(k))
        
    n=n*2           


