#include <igraph/igraph.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int count_white(int vid,igraph_t *g,int *visited)
{
                 igraph_vit_t vit3;
	     	 igraph_vs_t vs3;
		 igraph_vs_adj(&vs3,vid, IGRAPH_OUT);
		 igraph_vit_create(g, vs3, &vit3);
		 int count=0;
		 while (!IGRAPH_VIT_END(vit3)) {
		     
			if(visited[IGRAPH_VIT_GET(vit3)]==0)
			    count++;
		//	#pragma omp critical (vit3)
			IGRAPH_VIT_NEXT(vit3);
		}
		igraph_vit_destroy(&vit3);
		igraph_vs_destroy(&vs3);
		return count;
             
    
}

int find_min(igraph_t *g, int *visited, int *white)
{
    int i,j,max_vid=igraph_vcount(g),vid=0,counter=0;
	   // max_vid=igraph_vcount(g);
	    //igraph_vector_reverse_sort((igraph_vector_t*)&v);
	    //igraph_degree(g, &mdeg, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
	   // igraph_vector_int_print(&mdeg);
	   // #pragma omp parallel for num_threads(4) private(i) shared(max_vid,vid)
	    #pragma omp parallel for num_threads(4) //private(i) shared(visited)
	    for(i=0;i<igraph_vcount(g);i++)
	    {
	    	visited[i]=0; 
	    	
	    		    	
	    }
	    #pragma omp parallel for num_threads(4)// private(i) shared(max_vid,vid,visited,white)
	    for(i=0;i<igraph_vcount(g);i++)
	    {	
	        white[i]=count_white(i,g,visited);
	        if(white[i]==0)
	        {
	           visited[i]=1;
	           counter++;
	        }
	    	if(max_vid>white[i])
	    	{
	    		max_vid=white[i];
	    		vid=i;
	    	}
	    	//dom_set[i]=-1;
	   }
	  // printf("mm:%d",max_vid);
	   //exit(1);
	   return max_vid;
}

int bfs(int vid, int level, igraph_t *g, int *white,int *visited)
{
	         igraph_vit_t vit3,vit4,vit5;
	     	 igraph_vs_t vs3,vs4,vs5;
		 igraph_vs_adj(&vs3,vid, IGRAPH_OUT);
		 //printf("vid=%d,%d,%d,%d\n",vid,white[vid],visited[vid],white[vid]);
		 igraph_vit_create(g, vs3, &vit3);
		 int count=white[vid];
		 int win=vid;
		 while (!IGRAPH_VIT_END(vit3) && count>0) {
		 
		
		     
			if(visited[IGRAPH_VIT_GET(vit3)]!=1 && white[IGRAPH_VIT_GET(vit3)]>count)
			{
			     	win=IGRAPH_VIT_GET(vit3);
			     	count=white[IGRAPH_VIT_GET(vit3)];
			     	//printf("win=%d,%d,%d\n",win,vid,count);
			     	
			}
			igraph_vs_adj(&vs4,IGRAPH_VIT_GET(vit3),IGRAPH_OUT);
		 	igraph_vit_create(g, vs4, &vit4);
		 	while (!IGRAPH_VIT_END(vit4)) {
		 	
		 	   if(visited[IGRAPH_VIT_GET(vit4)]!=1 && white[IGRAPH_VIT_GET(vit4)]>count)
			   {
			     	win=IGRAPH_VIT_GET(vit4);
			     	count=white[IGRAPH_VIT_GET(vit4)];
			     	//printf("win1=%d\n",win);
			     	
			   }	
		 	   IGRAPH_VIT_NEXT(vit4);
		 	
		 	}
			
			IGRAPH_VIT_NEXT(vit3);
			igraph_vit_destroy(&vit4);
			igraph_vs_destroy(&vs4);
		}
		igraph_vit_destroy(&vit3);

		igraph_vs_destroy(&vs3);

		if(win==vid && white[vid]>0)
		{
		  // white[vid]=0;
		   visited[vid]=1;
		   igraph_vs_adj(&vs5,vid, IGRAPH_OUT);
		   igraph_vit_create(g, vs5, &vit5);
		   while (!IGRAPH_VIT_END(vit5)) {
		   	
		   	visited[IGRAPH_VIT_GET(vit5)]=2;
		   	IGRAPH_VIT_NEXT(vit5);
		   }
		  igraph_vit_destroy(&vit5);
		  igraph_vs_destroy(&vs5);
		
		 //  printf("vid=%d",vid);
		   return vid;
		}  
		else
		   return -1;   
		
}

void find_dom(igraph_t *g,int *visited)
{
	    
	    int counter=0,vertex,flag=0;
	    
	    int *white=(int*)malloc(igraph_vcount(g)*sizeof(int));
	    int *vertex_id=(int*)malloc(igraph_vcount(g)*sizeof(int));
	   
	    //igraph_integer_t mdeg;
	    //igraph_vector_int_init(&v, igraph_vss_all());
	    int i,j;	    
            int deg=find_min(g,visited,white);
            printf("deg:%d\t",deg);
	    #pragma omp parallel for num_threads(4) 
	    for(i=0;i<igraph_vcount(g);i++)
	    {
	    	visited[i]=0; 
	    	white[i]=count_white(i,g,visited);
	    	//dom_set[i]=-1;
	    }
	   // int count2=0;	
	    	
	    while(1)
	    {
	    //printf("sub");
	    flag=0;
	     #pragma omp parallel for num_threads(4) 
	     for(j=0;j<igraph_vcount(g);j++)
	     {
	         int tid = omp_get_thread_num();
	         if(white[j]>0)
	         {
	           
	         	white[j]=count_white(j,g,visited);
//	         	#pragma omp critical (flag)
	         	if(white[j]>0)
	         	{
	             	flag=1;
	         	}
	         }
	         if(visited[j]==0 && white[j]==0)
	         {
	            visited[j]=1;
	            counter++;
	         }
	
	         //bfs(j,2,g,visited);
	         
	         //printf("j=%d,w:%d,%d,%d\n",j,white[j],tid,flag);
	     }
	     //#pragma omp critical
	     if(flag==0)
	     {
	         printf("ok");
	         break;
	     }
	     else
	        flag=0;
	     #pragma omp parallel for num_threads(4) 
	     for(j=0;j<igraph_vcount(g);j++)
	     {
	     
	       int tid = omp_get_thread_num();
	       
	       
	       if(visited[j]!=1 && white[j]>0)
	       {
	          // #pragma omp critical (vertex)
	           vertex=bfs(j,2,g,white,visited);
	           if(vertex==j)
	             //  printf("v:%d,%d\n",vertex,j);
	          
	         // #pragma omp critical (vertex)
	           if(vertex!=-1)
	           {
	                //printf("v:%d,%d\n",vertex,j);
	                //dom_set[counter]=vertex;
	                visited[vertex]=1;
	                //#pragma omp critical (counter)
	                counter++;
	            //    printf("counter=%d,%d\n",vertex,counter);
	           }
	      }
	     }
	     //break;
	     }


	    
}
int main(int argc, char *argv[]) {
  
 	 igraph_t g;
	 int *height;	
	 clock_t t,e; 
         unsigned long int totalBytes;
         srand(time(NULL));         
 	 igraph_integer_t eid,from,to,value,vid,eid1,flow,source,sink,count=0;
 	 igraph_set_attribute_table(&igraph_cattribute_table);
	 //omp_set_num_threads(2000);
	 
	 printf("%d\n",omp_get_max_threads());
	 FILE *fp=fopen(argv[1],"r"); 
	 FILE *fp1=fopen(argv[2],"a"); 
 	 //omp_set_num_threads(50000) ;
 	 if(!fp)
 	 {
	
		perror("file does not exist\n");
		exit(1);
 	 }
	
 	 igraph_read_graph_edgelist(&g, fp, 0, IGRAPH_UNDIRECTED);
 	 int *visited=(int*)malloc(igraph_vcount(&g)*sizeof(int));
	 
	 int start=clock();
 	 
           	 
 	 find_dom(&g,visited);
 	 int end=clock();
 	 fclose(fp);
	 //height=(int*)calloc(igraph_vcount(&g),sizeof(int));
 	 int i,counter=0;
 	 for(i=0;i<igraph_vcount(&g);i++)
	 {
	    	if(visited[i]==1)
	    	   counter++;
	    	
	    	
	    	
	 }
         //printf("%d",find_min_deg);
 	 fprintf(fp1,"%d,%d,%d,%fs\n",igraph_vcount(&g),igraph_ecount(&g),counter++,((double) (end - start)) / CLOCKS_PER_SEC);
         //printf("%d,time:%fs",counter,((double) (end - start)) / CLOCKS_PER_SEC);
         fclose(fp1);
    	 //igraph_destroy(&g);
     	 return 0;
}
