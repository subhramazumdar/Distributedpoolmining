
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
		 
		 //    count2++;
		    //if(IGRAPH_VIT_GET(vit3)==12800)
			//printf(" g:%d\n",IGRAPH_VIT_GET(vit3));
		//	#pragma omp critical (visited)
		     
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
	    #pragma omp parallel for num_threads(4)
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

int find_max_deg(igraph_t *g, int *visited, int *white)
{
	
            
	   
	    //igraph_integer_t mdeg;
	    //igraph_vector_int_init(&v, igraph_vss_all());
	    int i,j,max_vid=-1,vid=0;
	   // max_vid=igraph_vcount(g);
	    //igraph_vector_reverse_sort((igraph_vector_t*)&v);
	    //igraph_degree(g, &mdeg, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
	   // igraph_vector_int_print(&mdeg);
	    #pragma omp parallel for num_threads(4) //private(i) shared(visited) 
	    for(i=0;i<igraph_vcount(g);i++)
	    {
	     if(visited[i]!=0)
	    	visited[i]=0; 
	    	
	    	
	    	
	    }
	    #pragma omp parallel for num_threads(4) //private(i) shared(max_vid,vid,white,visited) 
	    for(i=0;i<igraph_vcount(g);i++)
	    {	
	      white[i]=count_white(i,g,visited);
	    	if(max_vid<white[i])
	    	{
	    		max_vid=white[i];
	    		vid=i;
	    	}
	    	//dom_set[i]=-1;
	   }
	  // printf("mm:%d",max_vid);
	   //exit(1);
	   return vid;
}
int bfs(int *store, int **store_vert, igraph_t *g, int m,int *level,int diameter)
{

        int k=0,l=0,j=0,flag=0;
       
       //#
	while(store[j]!=-1)
	{
	
	       igraph_vit_t vit3;
	       igraph_vs_t vs3;
	 //      printf("%d,%d,%d,diam:%d,%d\n",store[j],m,j,diameter,level[store[j]]);
	         //printf("sss");
		 igraph_vs_adj(&vs3,store[j], IGRAPH_OUT);
		 
		 //printf("vid=%d,%d,%d,%d\n",vid,white[vid],visited[vid],white[vid]);
		 igraph_vit_create(g, vs3, &vit3);
		 
                 
		 //int count=white[vid];
		 //int win=vid;
		 while (!IGRAPH_VIT_END(vit3)) {
		 
		 //    count2++;
		    //if(IGRAPH_VIT_GET(vit3)==12800)
			//printf(" g:%d,%d\n",IGRAPH_VIT_GET(vit3),white[IGRAPH_VIT_GET(vit3)]);
		//	#pragma omp critical (visited)
	//	printf("v=%d,%d\t",IGRAPH_VIT_GET(vit3),level[IGRAPH_VIT_GET(vit3)]);
		     if(level[IGRAPH_VIT_GET(vit3)]==-1)
		     {
			level[IGRAPH_VIT_GET(vit3)]=m+1;
			store_vert[m+1][k]=IGRAPH_VIT_GET(vit3);
			flag=1;
			//printf("store=%d\t",IGRAPH_VIT_GET(vit3));
			k++;
			
		 	
		 	}
			
			
			//
		
		     
		     IGRAPH_VIT_NEXT(vit3);
		
		}
		
		igraph_vit_destroy(&vit3);
		
		igraph_vs_destroy(&vs3);
		
		j++;	
		
	}
        if(flag==0)
          return m;

	
	store_vert[m+1][k]=-1;
	j=0;
	igraph_vit_t vit4;
	igraph_vs_t vs4;
        flag=0;
        l=0;
       // #pragma omp parallel 
	while(store_vert[m+1][j]!=-1)
	{               
	
	                igraph_vs_adj(&vs4,store_vert[m+1][j],IGRAPH_OUT);
		 	igraph_vit_create(g, vs4, &vit4);
		 	while (!IGRAPH_VIT_END(vit4)) {
		 	       
		 		//printf("l:%d,%d\n",level[IGRAPH_VIT_GET(vit4)],m+1);
		 		 if(level[IGRAPH_VIT_GET(vit4)]==-1)
		 		 {
		 		   flag=1;
		 		   level[IGRAPH_VIT_GET(vit4)]=m+2;	
		 		   store_vert[m+2][l]=IGRAPH_VIT_GET(vit4);
		 		  // printf("s2=%d\t",IGRAPH_VIT_GET(vit4));
		 		   l++;
		 	   
		 		 }
		 		 IGRAPH_VIT_NEXT(vit4);
			}
			j++;
			igraph_vit_destroy(&vit4);
			igraph_vs_destroy(&vs4);
	
	}

     if(flag==1)
     {	
       if(m+2<=diameter)
       {
       	    //  printf("m:%d,l:%d",m+2,l);	
		store_vert[m+2][l]=-1;
		
	}
		
	j=0;	
	l=0;
       if(m>0 && m+3<=diameter)
       {
           //printf("diam");
           flag=0;
          // #pragma omp parallel private(j,l)
                //  #pragma omp parallel 
             while(store_vert[m+2][j]!=-1)
	{               
	
	                igraph_vs_adj(&vs4,store_vert[m+2][j],IGRAPH_OUT);
		 	igraph_vit_create(g, vs4, &vit4);
		 	//printf("ss%d",store_vert[m+2][j]);
		 	while (!IGRAPH_VIT_END(vit4)) {
		 	
		 	if(level[IGRAPH_VIT_GET(vit4)]==-1)
		 	{
		 	  flag=1;
		 	   level[IGRAPH_VIT_GET(vit4)]=m+3;	
		 	   store_vert[m+3][l]=IGRAPH_VIT_GET(vit4);
		 	//  printf("s2=%d\t",store_vert[m+3][l]);
		 	   l++;
		 	   
		 	 }
		
		 	   IGRAPH_VIT_NEXT(vit4);
	}
	
		j++;
		igraph_vit_destroy(&vit4);
		igraph_vs_destroy(&vs4);
	
	}
         if(flag==0)
        {
  //        printf("golu"); 
	   return m+2;	
        }
         else
            store_vert[m+3][l]=-1;   
       
       }
       printf("diam:%d",diameter);
       return diameter;	
       }
       else 
       {
       		return m+1;
       }	
	
	
}
int add_dom(igraph_t *g, int **store_vert,int level,int *white,int *visited,int *count,int param)
{

	int j=0,count_dom=0,vid_select,max_count=0;
	igraph_vit_t vit4;
	igraph_vs_t vs4;
	int d=0;
        //printf("level=%d\n",level);
        
        if(level==0)
		{
		       printf("Dom=%d\n",store_vert[level][0]);
		       igraph_vs_adj(&vs4,store_vert[level][0],IGRAPH_OUT);
		       igraph_vit_create(g, vs4, &vit4);

			visited[store_vert[level][0]]=1;
			white[store_vert[level][0]]=0;
			
		 	while (!IGRAPH_VIT_END(vit4)) {
		 	
		 	  if(visited[IGRAPH_VIT_GET(vit4)]==0)
		 	    count_dom++;
		 	   visited[IGRAPH_VIT_GET(vit4)]=2;
		 	   //printf("d:%d\n",IGRAPH_VIT_GET(vit4));
		 	   IGRAPH_VIT_NEXT(vit4);
		 	   
			}
			count_dom++;
			*count=*count+1;
			igraph_vit_destroy(&vit4);
   		        igraph_vs_destroy(&vs4);
   		        
   		        /*igraph_vs_adj(&vs4,store_vert[level][0],IGRAPH_OUT);
 		        igraph_vit_create(g, vs4, &vit4);
 		        while (!IGRAPH_VIT_END(vit4)) {
 		        	
 		        	white[IGRAPH_VIT_GET(vit4)]=count_white(IGRAPH_VIT_GET(vit4),g,visited);
 		        	IGRAPH_VIT_NEXT(vit4);
 		        	
 		        
 		        }
 		        igraph_vit_destroy(&vit4);
   		        igraph_vs_destroy(&vs4);*/
 		        
			
		 	
			
		}
		else
		{
      int k=0;
      d=0;
      //#pragma omp parallel for num_threads(4) 
      for(d=0;d<param;d++)
      { 
       
        max_count=0;
        j=0;
	while(store_vert[level][j]!=-1)
	 //parallel num_threads(4) shared(white,visited)
	{ 
	   // printf("store=%d\n",store_vert[level][j]);
	    if(white[store_vert[level][j]]!=0)
	     {
	      	white[store_vert[level][j]]=count_white(store_vert[level][j],g,visited);
	      if(max_count<white[store_vert[level][j]])
		      {
		          
		      	  max_count=white[store_vert[level][j]];
		      	  vid_select=store_vert[level][j];
		      }
		   }
		      if(white[store_vert[level][j]]==0 && visited[store_vert[level][j]]==0)
		      { 
		      		  visited[store_vert[level][j]]=1;
		      		  count_dom++;
		      		  *count=*count+1;
           //                   d++;
		      
		      }
		
		j++;
	}
	
//	#pragma omp barrier
	if(max_count>0)
	{
		igraph_vs_adj(&vs4,vid_select,IGRAPH_OUT);
	        igraph_vit_create(g, vs4, &vit4);
	        if(visited[vid_select]==0)
	        {
	            count_dom++;
	            
	        }
	        *count=*count+1;
	        visited[vid_select]=1;
	        white[vid_select]=0;
	        
		while (!IGRAPH_VIT_END(vit4)) {
		 	   visited[IGRAPH_VIT_GET(vit4)]=2;
		 	
		 	   IGRAPH_VIT_NEXT(vit4);
		}
		igraph_vit_destroy(&vit4);
   		igraph_vs_destroy(&vs4);
		count_dom+=max_count;
             // d++;
	
	}
//        d--;
//	else
//	   break;
	
	
	}
	
        }
     
	return count_dom;
}

int find_dom_cmm(igraph_t *g, int vid_max,int *result,int *visited, int *white,int param)
{
        int *level=(int*)malloc(igraph_vcount(g)*sizeof(int)); 
        int cluster=abs(*result/3)+1;
        int i;
        for(i=0;i<igraph_vcount(g);i++)
        {
        	level[i]=-1;
        }
       
        //int level_0=vid_max; 
        level[vid_max]=0;
        int p=*result;
        int d=p;
    //    printf("%d,%d\n",*result,d);
        
        int *store_vert[p+1];
       // int *ref_vert[cluster][igraph_vcount(g)];
        //int *ref_vert[cluster];
        for(i=0;i<=p;i++)
        {
        	store_vert[i]=(int*)malloc(igraph_vcount(g)*sizeof(int));
        	
        }
        
        /*for(j=0;j<cluster;j++)
        {
        	for(i=0;i<igraph_vcount(g);i++)
        	{
 	       		ref_vert[j][i]=(int*)malloc(igraph_vcount(g)*sizeof(int));
 	       	}
 	}*/

        store_vert[0][0]=vid_max;
        store_vert[0][1]=-1;
        
        int m=0,k=0;
        
        for(i=0;i<cluster;i++)
	{
				
				
			        //
			        if(m<p)
			        {
				   d=bfs(store_vert[m],store_vert,g,m,level,p);
				  // printf("d=%d\n",d);
				   p=d;
				}
			
				m=3*(i+1)-1;
				//count[i]=j;
			
	}
        int count=0,j;
  
	int slide=0,halt=0,n=0,count_dom=0;
	
	//p=d;
	
	cluster=abs(p/3)+1;
	int n_prev=0;
	while(n<igraph_vcount(g))
	{
	        
		
		#pragma omp parallel for num_threads(4) //shared(white,visited) private(i)
		for(i=0;i<cluster;i++)
		{
		
		     //printf("i:%d,%d,%d:\n",i,p,slide);
			if(3*i+slide<=p)
			{
			     //  #pragma openmp critical (count_dom)
			       int k=add_dom(g,store_vert,3*i+slide,white,visited,&count_dom,param);
			       
				n=n+k;
				//printf("slide:%d,%d,%d\n",k,n,3*i+slide);
			}
			
			
				
			
			
		
		}
		slide=(slide+1);
		int counter=0;
		
		
		if(slide==p+1)
		{
		
		
		  slide=1;
	    	        
	    	        
	    	//        if(counter==0)
          	
		
		  
		 /* if(n_prev-n==0)
		  {  
		  printf("ok");
		      #pragma omp parallel for num_threads(4) shared(visited) private(i)
		for(i=0;i<igraph_vcount(g);i++) 
		 {
	    	
	    	
	    	   if(visited[i]!=1)
	    	   {
	    	   
	    	        white[i]=count_white(i,g,visited);
	    	        if(white[i]==0 && visited[i]==0)
	    	        {
	    	            visited[i]=1;
	    	            count_dom++;
	    	        }
	    	        else
	    	            counter++;
	    	        }
	    	        }
	    	        
	    	        }
	    	        //#pragma omp parallel for num_threads(4) shared(visited) private(i)
	    	        
		  	
		  
		
	    	       

	    	   
		    		
	    	n_prev=n;*/
		
		  
		}
		
		
		
	
	    	
	    	
	    	
	 }
	 
	 //printf("c:%d\n",counter);
	   
	
	
	
	
	printf("count=%d,%d,%d,%f\n",count_dom,n,p,*result);
	return count_dom;
	
}


int main(int argc, char *argv[]) {
  
 	 igraph_t g;
	 int *height,vid_max;	
	 clock_t t,e; 
         unsigned long int totalBytes;
         srand(time(NULL));         
 	 igraph_integer_t eid,from,to,value,vid,eid1,flow,source,sink,count=0;
 	 igraph_real_t result;
 	 igraph_set_attribute_table(&igraph_cattribute_table);
 	
	 //omp_set_num_threads(2000);
	 //printf("%d\n",omp_get_max_threads());
	 
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
	 int *white=(int*)malloc(igraph_vcount(&g)*sizeof(int));
	// igraph_diameter(&g, &result, 0, 0, 0, 0, IGRAPH_DIRECTED, 1);
	 int min_deg=0;
	 int find_min_deg=find_min(&g,visited,white);
	 int run=atoi(argv[3]);
	 int start=clock();
 	
	

 	 
 	 vid_max=find_max_deg(&g,visited,white);
 	 int k=20;
 	 //printf("result:%d\n",k);
 	 int param=(9*igraph_vcount(&g)*igraph_vcount(&g))/(igraph_ecount(&g)*k*run);
         //param=3;
 	//printf("%d,%d",param,find_min_deg);
 	 int dom_count=find_dom_cmm(&g,vid_max,&k,visited,white,param);
 	 int end=clock();
 	 dom_count=0;
 	 fclose(fp);
 	 int i;
 	  for(i=0;i<igraph_vcount(&g);i++)
	 {
	    	if(visited[i]==1)
	    	   dom_count++;
	    	if(visited[i]==0)
	    	{
	    	   printf("oookk");
	    	}
	    	
	    	
	 }
	 //height=(int*)calloc(igraph_vcount(&g),sizeof(int));
	 //printf("%d,%d,%fs",dom_count,find_min_deg,((double) (end - start)) / CLOCKS_PER_SEC);
 	  
         fprintf(fp1,"%d,%d,%d,%f,%d,%fs\n",igraph_vcount(&g),igraph_ecount(&g),find_min_deg,igraph_vcount(&g)*((1+log(1+find_min_deg))/(1+find_min_deg)),dom_count,((double) (end - start)) / CLOCKS_PER_SEC);
    	 //igraph_destroy(&g);
     	 return 0;
}
