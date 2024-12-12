#include <igraph/igraph.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>

int max_degree = -1;
int vertex_count = 0;

// Pre-allocated vectors for neighbor queries
static igraph_vector_int_t neighbors;
static int initialized = 0;

// Initialize global resources
void init_resources() {
    if (!initialized) {
        igraph_vector_int_init(&neighbors, 0);
        initialized = 1;
    }
}

// Cleanup global resources
void cleanup_resources() {
    if (initialized) {
        igraph_vector_int_destroy(&neighbors);
        initialized = 0;
    }
}

// Optimized white node counter using pre-allocated vector
int count_white_new(int vid, const igraph_t *g, const int *visited) {
    igraph_neighbors(g, &neighbors, vid, IGRAPH_ALL);
    int count = 0;
    int len = igraph_vector_int_size(&neighbors);
    
    #pragma omp simd reduction(+:count)
    for (int i = 0; i < len; i++) {
        count += (visited[VECTOR(neighbors)[i]] == 0);
    }
    return count;
}

// Optimized modified counter
int count_mod(int vid, const igraph_t *g, const int *visited, const int *blue) {
    igraph_neighbors(g, &neighbors, vid, IGRAPH_ALL);
    int count = 0;
    int len = igraph_vector_int_size(&neighbors);
    
    #pragma omp simd reduction(+:count)
    for (int i = 0; i < len; i++) {
        int neighbor = VECTOR(neighbors)[i];
        count += (visited[neighbor] == 0 && blue[neighbor] == 0);
    }
    return count;
}

// Optimized minimum finder with degree consideration
int find_min(igraph_t *g, int *visited, int *white) {
    const int n = igraph_vcount(g);
    int max_vid = n;
    int vid = 0;
    int counter = 0;
    
    memset(visited, 0, n * sizeof(int));
    
    #pragma omp parallel for schedule(guided)
    for(int i = 0; i < n; i++) {
        white[i] = count_white_new(i, g, visited);
        if(white[i] == 0) {
            visited[i] = 1;
            counter++;
        }
        #pragma omp critical
        {
            if(max_vid > white[i]) {
                max_vid = white[i];
                vid = i;
            }
        }
    }
    return max_vid;
}

// Optimized BFS implementation with degree prioritization
int bfs(int vid, igraph_t *g, int *white, int *visited, int *temp_white, int **white_deg) {
    if (visited[vid] == 1) return -1;
    
    igraph_neighbors(g, &neighbors, vid, IGRAPH_ALL);
    temp_white[vid] = white[vid];
    int max_deg = 0,flag2=0;
    
    // Find maximum degree efficiently
    for(int k = white[vid] - 1; k > 0 && max_deg < k; k--) {
        if(white_deg[vid][k] != -1) {
            max_deg = k;
            break;
        }
    }
    
    const int len = igraph_vector_int_size(&neighbors);
    
    // Process neighbors in parallel
    //#pragma omp parallel for schedule(static) reduction(-:temp_white[vid])
    for(int i = 0; i < len; i++) {
        int neighbor = VECTOR(neighbors)[i];
        if(visited[neighbor] != 1) {
            if(visited[neighbor] == 0 && (white[neighbor] > white[vid] || 
               (white[neighbor] == white[vid] && neighbor > vid))) {
                temp_white[vid]--;
                //continue;
            }
            else
            {
               flag2=0;
            }
            for(int k=white[vid]-1;k>0 && max_deg<k;k--)
	    {
		                 if(white_deg[neighbor][k]!=-1)
		                 {
		                       if(max_deg<k)
		                       {
		                           max_deg=k;
		                       }
		                       break;
		                 }
            }
            
            if(flag2==0 && visited[neighbor] == 0) {
                for(int k = white[vid]; k <= max_degree; k++) {
                    if(white_deg[neighbor][k] != -1) {
                        if((k == white[vid] && white_deg[neighbor][white[vid]] > vid) || k > white[vid]) {
                            temp_white[vid]--;
                            break;
                        }
                    }
                }
            }
            flag2=0;
        }
    }
    
    // Select vertex based on degree and white count
    if(temp_white[vid] > 0 && temp_white[vid] >= max_deg) {
        visited[vid] = 1;
        white[vid] = 0;
       // #pragma omp atomic
        vertex_count++;
        return vid;
    }
    return -1;
}

void find_dom(igraph_t *g, int *visited) {
    const int n = igraph_vcount(g);
    int *white = (int*)calloc(n, sizeof(int));
    int *temp_white = (int*)calloc(n, sizeof(int));
    int **white_deg = (int**)malloc(n * sizeof(int*));
    
    // Initialize arrays
    memset(visited, 0, n * sizeof(int));
    
    // Calculate initial white counts and find max degree
    //#pragma omp parallel for reduction(max:max_degree)
    for(int i = 0; i < n; i++) {
        white[i] = count_white_new(i, g, visited);
        temp_white[i] = white[i];
        if(max_degree < white[i]) {
            max_degree = white[i];
        }
    }
    
    // Initialize white_deg arrays
    //#pragma omp parallel for
    for(int i = 0; i < n; i++) {
        white_deg[i] = (int*)calloc(max_degree + 1, sizeof(int));
        for(int k = 0; k <= max_degree; k++) {
            white_deg[i][k] = -1;
        }
    }
    
    // Initialize neighbor relationships
    //#pragma omp parallel for
    for(int i = 0; i < n; i++) {
        if(visited[i] != 1) {
            igraph_neighbors(g, &neighbors, i, IGRAPH_ALL);
            int len = igraph_vector_int_size(&neighbors);
            for(int j = 0; j < len; j++) {
                int neighbor = VECTOR(neighbors)[j];
                if(visited[neighbor] != 1) {
                    white_deg[i][white[neighbor]] = neighbor;
                }
            }
        }
    }
    
    // Main dominating set algorithm
    while(1) {
        int flag = 0;
        int breaking = 0;
        
        // Process vertices in parallel
       //#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < n; i++) {
            if(visited[i] != 1 && white[i] > 0) {
         //       #pragma omp atomic write
                flag = 1;
                int vertex = bfs(i, g, white, visited, temp_white, white_deg);
            }
        }
        
        if(!flag) break;
        
        // Update white counts
        //#pragma omp parallel for
        for(int i = 0; i < n; i++) {
            if(visited[i] == 1) {
                white[i] = 0;
                igraph_neighbors(&(*g), &neighbors, i, IGRAPH_ALL);
                int len = igraph_vector_int_size(&neighbors);
                for(int j = 0; j < len; j++) {
                    int neighbor = VECTOR(neighbors)[j];
                    if(visited[neighbor] == 0) {
                        visited[neighbor] = 2;
                    }
                }
            }
        }
        
        // Update white counts and check for termination
        //#pragma omp parallel for reduction(|:breaking)
        for(int i = 0; i < n; i++) {
            if(white[i] > 0) {
                white[i] = count_white_new(i, g, visited);
                if(white[i] > 0) breaking = 1;
            }
        }
        
        if(!breaking) break;
        
        // Reset and update white_deg
        //#pragma omp parallel for collapse(2)
        for(int i = 0; i < n; i++) {
            for(int k = 0; k <= max_degree; k++) {
                white_deg[i][k] = -1;
            }
        }
        
       // #pragma omp parallel for
        for(int i = 0; i < n; i++) {
            if(visited[i] != 1) {
                igraph_neighbors(g, &neighbors, i, IGRAPH_ALL);
                int len = igraph_vector_int_size(&neighbors);
                for(int j = 0; j < len; j++) {
                    int neighbor = VECTOR(neighbors)[j];
                    if(visited[neighbor] != 1) {
                        white_deg[i][white[neighbor]] = neighbor;
                    }
                }
            }
        }
    }
    
    // Cleanup
    for(int i = 0; i < n; i++) {
        free(white_deg[i]);
    }
    free(white_deg);
    free(white);
    free(temp_white);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    igraph_t g;
    clock_t start, end;
    
    igraph_set_attribute_table(&igraph_cattribute_table);
    omp_set_num_threads(omp_get_num_procs());
    
    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        perror("Error opening input file");
        return 1;
    }
    
    igraph_read_graph_edgelist(&g, fp, 0, IGRAPH_UNDIRECTED);
    fclose(fp);
    
    int *visited = (int*)calloc(igraph_vcount(&g), sizeof(int));
    int *mark = (int*)calloc(igraph_vcount(&g), sizeof(int));
    
    init_resources();
    
    start = clock();
    find_dom(&g, visited);
    end = clock();
    
    int counter = 0, eff_count = 0;
    for(int i = 0; i < igraph_vcount(&g); i++) {
        if(visited[i] == 1) {
            counter++;
            if(mark[i] == 0) {
                mark[i] = 1;
                eff_count++;
            }
            
            igraph_neighbors(&g, &neighbors, i, IGRAPH_ALL);
            int len = igraph_vector_int_size(&neighbors);
            for(int j = 0; j < len; j++) {
                int neighbor = VECTOR(neighbors)[j];
                if(mark[neighbor] == 0) {
                    eff_count++;
                    mark[neighbor] = 1;
                }
            }
        }
    }
    
    FILE *fp1 = fopen(argv[2], "a");
    if(fp1) {
        fprintf(fp1, "%ld,%ld,%d,%d,%ld,%f\n",
                (long)igraph_vcount(&g), (long)igraph_ecount(&g), counter, eff_count,
                (long)(igraph_vcount(&g) / max_degree),
                ((double)(end - start)) / CLOCKS_PER_SEC);
        fclose(fp1);
    }
    
    cleanup_resources();
    igraph_destroy(&g);
    free(visited);
    free(mark);
    
    return 0;
}
