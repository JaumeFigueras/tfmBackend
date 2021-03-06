// C / C++ program for Dijkstra's shortest path algorithm for adjacency
// list representation of graph

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>
#define MAX 3007

int nodes[MAX];

// A structure to represent a node in adjacency list
struct AdjListNode
{
    int dest;
    double weight;
    struct AdjListNode* next;
};
struct PrecNodeDist
{
    int pred;
    double weight;
};


PrecNodeDist pred[MAX][MAX];

// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, double weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, double weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

// A utility function used to print the solution
void printArr(double dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %f\n", i, dist[i]);
}

// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src)
{
    int V = graph->V;// Get the number of vertices in graph
    double dist[V];      // dist values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    pred[src][src].weight=0.0;

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                                          pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                pred[src][v].pred = u;
                pred[src][v].weight=pCrawl->weight;

                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
    //printArr(dist, V);
}

void memorize(char file[])
{
	int i,j,n,u;
	double dist;

    FILE * fp;
    char * line = NULL;
    char * idtoAll = NULL;
    char * idtoParse = NULL;
    char * idtoRest = NULL;
    char * id=NULL;
    int node1=0;
    int node2=0;
    //printf("%s\n",file);
	//int start=-172242;
	/*<-42392_To_-98444' v='0.252223132135303' />*/
//-53840_To_-172242
	//int ends[6]={-47998,-83910,-53840,-136928,-74118,-103972};

	int start=-172242;
	/*<-42392_To_-98444' v='0.252223132135303' />*/
//-53840_To_-172242
	int ends[6]={-47998,-83910,-53840,-136928,-74118,-103972};
    //long start=2106002804;
    //long ends[5]={1927119584, 3676415552, 1924871332, 2144062835, 1924892057};
    size_t len = 0;
    ssize_t read;
    len=0;
    //printf("%s\n",file);
    fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int numNode=0;
    int NumA=0;
    double minLat =0;
    double maxLat=0;
    double minLon=0;
    double maxLon=0;
    bool sorted=false;
    while ((read = getline(&line, &len, fp)) != -1) {

        if(strstr(line,"<node")!=NULL){
            idtoAll=strstr(line,"lon='");
            memmove(idtoAll, idtoAll+5, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"' ");
            idtoRest[0]='\0';
            //printf("%s\n",idtoAll);
            nodes[numNode].lon=atof(idtoAll);
            if(minLon==0) minLon=nodes[numNode].lon;
            if(minLon>nodes[numNode].lon) minLon=nodes[numNode].lon;
            if(maxLon<nodes[numNode].lon) maxLon=nodes[numNode].lon;
            idtoAll=strstr(line,"lat='");
            memmove(idtoAll, idtoAll+5, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"' ");
            idtoRest[0]='\0';
            //printf("%s\n",idtoAll);
            nodes[numNode].lat=atof(idtoAll);
            if(minLat==0) minLat=nodes[numNode].lat;
            if(minLat>nodes[numNode].lat) minLat=nodes[numNode].lat;
            if(maxLat<nodes[numNode].lat) maxLat=nodes[numNode].lat;

            idtoAll=strstr(line,"id='");
            memmove(idtoAll, idtoAll+4, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"' ");
            idtoRest[0]='\0';
            nodes[numNode].id=atoi(idtoAll);
            ++numNode;
  //<node id='-122474' action='modify' lat='41.55136910003' lon='1.78672119968' >
        }
        if(strstr(line,"<way")!=NULL){
            if(!sorted){
                //printf("lines read\n");
                sorted=true;
                std::sort(std::begin(nodes),std::end(nodes),acompare);

            }
            while(strstr(line,"<nd")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"ref='");
            memmove(idtoAll, idtoAll+5, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"'");
            idtoRest[0]='\0';
            node1=atoi(idtoAll);
            read = getline(&line, &len, fp);
            while(strstr(line,"<nd")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"ref='");
            memmove(idtoAll, idtoAll+5, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"'");
            idtoRest[0]='\0';
            node2=atoi(idtoAll);
            int found=0;
            node1=binary_search(node1,numNode-1, 0);
            node2=binary_search(node2,numNode-1, 0);
            /*
            for(int i =0; i<numNode;++i){
                if(nodes[i].id==node1){
                    node1=i;
                    ++found;
                }
                if(nodes[i].id==node2){
                    node2=i;
                    ++found;
                }
            }*/
            read = getline(&line, &len, fp);
            while(strstr(line,"k='Distance_From_")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"v='");
            memmove(idtoAll, idtoAll+3, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"' ");
            idtoRest[0]='\0';
            if(node1>=0 && node2>=0){
                SparceElement a;
                SparceElement b;
                a.line=node1;
                a.column=node2;
                a.dist=atof(idtoAll);
                int lineSparce=0;
                for(lineSparce=0; lineSparce<NumA;++lineSparce){
                    if(SparceCompare(a,G[lineSparce])){
                        SparceElement c=a;
                        a=G[lineSparce];
                        G[lineSparce]=c;
                    }
                }
                ++NumA;
                G[lineSparce]=a;
                b.line=node2;
                b.column=node1;
                b.dist=atof(idtoAll);
                for(lineSparce=0; lineSparce<NumA;++lineSparce){
                    if(SparceCompare(b,G[lineSparce])){
                        SparceElement c=b;
                        a=G[lineSparce];
                        G[lineSparce]=b;
                    }
                }
                ++NumA;
                G[lineSparce]=b;
            }

            //if(atof(idtoAll)!=0.0)
            //G[node1][node2] = G[node2][node1] = atof(idtoAll);

            /*printf("%d",numNode);
            printf("lat %f\n",nodes[node1].lat);
            printf("lon %f\n",nodes[node1].lon);
            printf("lon %d\n",node1);

            printf("lat %f\n",nodes[node2].lat);
            printf("lon %f\n",nodes[node2].lon);
            printf("lon %d\n",node2);
                G[node1][node2] = G[node2][node1] =distance_ll(nodes[node1],nodes[node2]);*/
            //NumA+=2;
        }
    }


    fclose(fp);
    if (line)
        free(line);
    //printf("readed\n");
    //std::sort(std::begin(G),std::end(G),SparceCompare);
    printf("sorted %d: %d %d , %d %d\n", NumA, G[0].line, G[0].column, G[1].line, G[1].column);
    //NumArestes=NumA;



    for(int i =0; i<numNode;++i){
        if(nodes[i].id==start){
            start=i;
        }
        if(nodes[i].id==ends[0]){
            ends[0]=i;
        }
        if(nodes[i].id==ends[1]){
            ends[1]=i;
        }
        if(nodes[i].id==ends[2]){
            ends[2]=i;
        }
        if(nodes[i].id==ends[3]){
            ends[3]=i;
        }
        if(nodes[i].id==ends[4]){
            ends[4]=i;
        }
    }
    printf("%d\n",start);
    printf("Start=%d\n",nodes[start].id);
    printf("numNodes %d numArestes %d\n",numNode,NumA);

	printf("done\n");
    for(int i =0; i<numNode;++i){
        dijkstra(numNode,i);
    }

    double dist0[5]={0,0,0,0,0};
    int dist2[5][2];
    for(int i =0; i<numNode;++i){
        for(int j =0; j<numNode;++j){
            if(j!=i){
                int nd1Id=nodes[i].id;
                int nd2Id=nodes[j].id;
                if (cost[i][j]>=INF){
                    for(int k=0;k<5;++k){
                        if(dist0[k]==0){
                            double auxd=dist0[k];
                            dist0[k]=cost[i][j];
                            dist2[k][0]=nd1Id;
                            dist2[k][1]=nd2Id;
                            k=4;
                        }
                        if(dist0[k]>cost[i][j]){
                            double auxd=dist0[k];
                            int auxn1=dist2[k][0];
                            int auxn2=dist2[k][1];
                            dist0[k]=cost[i][j];
                            dist2[k][0]=nd1Id;
                            dist2[k][1]=nd2Id;
                            nd1Id=dist2[k][0];
                            nd2Id=dist2[k][1];
                        }
                    }
                 }
            }
        }
    }
    printf("mininf dist\n");

    /*for(int k=0;k<5;++k){
        printf("%ld %ld\n",dist2[k][0],dist2[k][1]);

    }

	for(int i=0;i<5;i++){
			printf("\nDistance of node%d=%f",nodes[ends[i]].id,distance[ends[i]]);
			printf("\nPath=%d",ends[i]);
            int k=0;
            int oldId=ends[i];
			j=ends[i];
			printf("[");
			do
			{
				j=pred[start][j];
				oldId=j;
			    printf("(%f, %f),",nodes[j].lon,nodes[j].lat);
				printf("<-%d",nodes[j].id);
            ++k;
			}while(j!=start);
			printf("]");
			printf("\nnumNodes = %d\n",k);
	}*/
	//return 0;
}

int *routing(int end, int start){
    route[0]=start;
    //printf("%d %d end start\n",end,start);
    end=binary_search(end,MAX-1, 0);
    start=binary_search(start,MAX-1, 0);
    int k=1;
    int j=pred[end][start];
    do
    {
        j=pred[end][j];
        route[k]=nodes[j].id;
        ++k;
    }while(j!=end);
    return route;
}

// Driver program to test above functions
int main()
{
    // create the graph given in above fugure
    int V = MAX;
    struct Graph* graph = createGraph(V);

    FILE * fp;
    char * line = NULL;
    char * idtoAll = NULL;
    char * idtoParse = NULL;
    char * idtoRest = NULL;
    char * id=NULL;
    int node1=0;
    int node2=0;
	int start=-172242;
	int ends[6]={-47998,-83910,-53840,-136928,-74118,-103972};
    //void addEdge(struct Graph* graph, int src, int dest, int weight)
    int numNode=0;
    size_t len = 0;
    ssize_t read;
    fp = fopen("./connexioGRAPH.osm", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line,"<node")!=NULL){

            idtoAll=strstr(line,"id='");
            idtoParse=strstr(idtoAll,"-");
            idtoRest=strstr(idtoParse,"' ");
            idtoRest[0]='\0';
            nodes[numNode]=atoi(idtoParse);
            ++numNode;
  //<node id='-122474' action='modify' lat='41.55136910003' lon='1.78672119968' >
        }
        if(strstr(line,"<way")!=NULL){
            while(strstr(line,"<nd")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"ref='");
            idtoParse=strstr(idtoAll,"-");
            idtoRest=strstr(idtoParse,"' ");
            idtoRest[0]='\0';
            node1=atoi(idtoParse);
            read = getline(&line, &len, fp);
            while(strstr(line,"<nd")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"ref='");
            idtoParse=strstr(idtoAll,"-");
            idtoRest=strstr(idtoParse,"' ");
            idtoRest[0]='\0';
            node2=atoi(idtoParse);
            for(int i =0; i<numNode;++i){
                if(nodes[i]==node1){
                    node1=i;
                }
                if(nodes[i]==node2){
                    node2=i;
                }
            }
            read = getline(&line, &len, fp);
            while(strstr(line,"k='Distance_From_")==NULL){
                read = getline(&line, &len, fp);
            }
            idtoAll=strstr(line,"v='");
            memmove(idtoAll, idtoAll+3, strlen(idtoAll));
            idtoRest=strstr(idtoAll,"' ");
            idtoRest[0]='\0';
            //if(atof(idtoAll)!=0.0)
            //G[node1][node2] = G[node2][node1] = atof(idtoAll);
            addEdge(graph, node1, node2, atof(idtoAll));
        }
    }


    for(int i =0; i<numNode;++i){
        if(nodes[i]==start){
            start=i;
        }
        if(nodes[i]==ends[0]){
            ends[0]=i;
        }
        if(nodes[i]==ends[1]){
            ends[1]=i;
        }
        if(nodes[i]==ends[2]){
            ends[2]=i;
        }
        if(nodes[i]==ends[3]){
            ends[3]=i;
        }
        if(nodes[i]==ends[4]){
            ends[4]=i;
        }
        if(nodes[i]==ends[5]){
            ends[5]=i;
        }
        dijkstra(graph, i);
    }



    for(int i=0;i<6;i++){
			printf("\nPath=%d",nodes[i]);
            int k=0;
			int j=ends[i];
			do
			{
			    ++k;
				j=pred[start][j].pred;
				//printf("<-%d",nodesCoor[j].id);
				printf("<-%d",nodes[j]);
			    //printf("G %f",G[ends[i]][start]);
			}while(j!=start);

			printf("\nnumNodes = %d\n",k);
	}

    return 0;
}