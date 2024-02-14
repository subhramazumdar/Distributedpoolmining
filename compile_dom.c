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
int return_max(int *white,igraph_t *g,int *visited)
{
  int count=0,j;
  int vid=-1;
  for(j=0;j<igraph_vcount(g);j++)
   {
       if(white[j]>count && visited[j]==0)
       {
          vid=j;
          count=white[j];
       }
   }
   return vid;
}
int count_white(int vid,igraph_t *g,int *visited)
{
                 igraph_vit_t vit3;
	     	 igraph_vs_t vs3;
		 igraph_vs_adj(&vs3,vid, IGRAPH_OUT);
		 igraph_vit_create(g, vs3, &vit3);
		 int count=0;
		 while (!IGRAPH_VIT_END(vit3)) {
		 
		 //    count2++;
		    //if(IGRAPH_VIT_GET(vit3)==12800)
			//printf(" g:%d\n",IGRAPH_VIT_GET(vit3));
		//	#pragma omp critical (visited)
		     
			if(visited[IGRAPH_VIT_GET(vit3)]==0)
			    count++;
		//	#pragma omp critical (vit3)
			IGRAPH_VIT_NEXT(vit3);
		}
		igraph_vs_destroy(&vs3);
		igraph_vit_destroy(&vit3);
		return count;
             
    
}

void find_dom(igraph_t *g)
{
	    //igraph_vector_int_t v,mdeg;
	    //igraph_vector_t indices;
	    //igraph_vector_init(&indices,0);
	    //igraph_vector_int_init(&v, 0);
	    //igraph_vector_int_init(&mdeg, 0);
	    //igraph_vs_t vs,vs2;
	    //igraph_vs_all(&vs);
	    int *visited=(int*)malloc(igraph_vcount(g)*sizeof(int));
	    int *white=(int*)malloc(igraph_vcount(g)*sizeof(int));
	  //  igraph_vit_t vit,vit1,vit2;
	    //igraph_integer_t mdeg;
	    //igraph_vector_int_init(&v, igraph_vss_all());
	    int i,j;
	    //igraph_vit_create(g, vs, &vit);
	    //igraph_vit_create(g, indices, &vit1);
	    //igraph_degree(g, &v, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
	    //igraph_vector_reverse_sort((igraph_vector_t*)&v);
	    //igraph_degree(g, &mdeg, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
	   // igraph_vector_int_print(&mdeg);
	    
	    for(i=0;i<igraph_vcount(g);i++)
	    	visited[i]=0; 
	   
	     //for(i=0;i<igraph_vcount(g);i++)
	    	//visited[i]=0; 
	    int count2=0;	
	    	
	     
	     
		 
	
	     int count=0,k;
	     igraph_vit_t vit3;
	     igraph_vs_t vs3;
	     
	     while(1)
	     {
	     
	         for(k=0;k<igraph_vcount(g);k++)
	    {
	     white[k]=count_white(k,g,visited);
	     //printf("%d\n",white[k]);
	     
	     }
	        j=return_max(white,g,visited);
	       if(j==-1)
	         break;
	       if(visited[j]==0)
	       {
	     
             
		 count++;
		  //igraph_vs_1(&vs,vertex_id[j]);
		 // #pragma omp critical (visited)
		  visited[j]=1;
		 // printf("v:%d,%d\n",j,white[j]);
		
		 // printf("%d,%d\n",vertex_id[j],j);
	         igraph_vs_adj(&vs3, j, IGRAPH_OUT);
		 igraph_vit_create(g, vs3, &vit3);
		 //#pragma omp section
		 {
		 while (!IGRAPH_VIT_END(vit3)) {
		 
		 //    count2++;
		    //if(IGRAPH_VIT_GET(vit3)==12800)
			//printf(" g:%d",IGRAPH_VIT_GET(vit3));
		//	#pragma omp critical (visited)
			visited[IGRAPH_VIT_GET(vit3)]=1;
		//	#pragma omp critical (vit3)
			IGRAPH_VIT_NEXT(vit3);
		}
		igraph_vs_destroy(&vs3);
		igraph_vit_destroy(&vit3);
             }
            
		}
		
		//#pragma omp critical (j)
		     	     
	     }
	    // double c=log(1+count2);
	     //printf("count2=%f,%d\n",(igraph_vcount(g)*c)/(1+count2),count);	
	     
	     //igraph_vector_permute(&indices,&v);
	  // for(i=0;i<igraph_vcount(g);i++)
	    //   printf("%d,%d\n",i,igraph_vector_get(&indices,i));
	     //igraph_vss_vector(&indices);

	    
}
int main(int argc, char *argv[]) {
  
 	 igraph_t g;
	 int *height;	
	clock_t t,e; 
        unsigned long int totalBytes;
         srand(time(NULL));         
 	 igraph_integer_t eid,from,to,value,vid,eid1,flow,source,sink,count=0;
 	 igraph_set_attribute_table(&igraph_cattribute_table);
 	 t=clock();
	 //omp_set_num_threads(100);
 	 FILE *fp=fopen(argv[1],"r"); 
 	 if(!fp)
 	 {
	
		perror("file does not exist\n");
		exit(1);
 	 }
	
 	 igraph_read_graph_edgelist(&g, fp, 0, IGRAPH_DIRECTED);
 	 find_dom(&g);
 	 fclose(fp);
	 //height=(int*)calloc(igraph_vcount(&g),sizeof(int));
 	  e=clock();
         printf("%d,%d,time:%fs",igraph_vcount(&g),igraph_ecount(&g),((double) (e - t)) / CLOCKS_PER_SEC);
    	 //igraph_destroy(&g);
     	 return 0;
}
