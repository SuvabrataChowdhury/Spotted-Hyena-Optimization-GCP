#ifndef __GRAPH__
	#define __GRAPH__

	//Graph is represented with adjacency matrix of type bool with the following convension,
	// adj[i][j] = 1 if there is an edge between vertex i and j,
	// adj[i][j] = 0 otherwise
	typedef struct Graph{
		int numVertices;
		int numEdges;
		int knownChromaticNum;

		int maxDegree;

		bool **adj;
	}Graph;

	//Initialize the graph from the given file with DIMACS format
	void getGraphInfo(FILE *file, Graph *graph){
		//Scan the first line to obtain known chromatic number, |V|, |E|
		fscanf(file,"%d %d %d",&graph->knownChromaticNum,&graph->numVertices,&graph->numEdges);
		
		//As adj is a matrix first initialize the array of pointers
		graph->adj = (bool**)calloc(graph->numVertices,sizeof(bool*));

		//then for each element of such array allocate memory for another pointer
		// pointing to bool type elements. Essentially creating an array of arrays
		for(int i=0;i<(graph->numVertices);i++)
			graph->adj[i] = (bool*)calloc(graph->numVertices,sizeof(bool));

		//Keep on scanning the remaining lines of the file until EOF is reached
		int v1,v2;
		while((fscanf(file,"%*s %d %d",&v1,&v2))==2){
			//As graph is undirected adj is a symmetric matrix
			if(!graph->adj[v1-1][v2-1]){
				graph->adj[v1-1][v2-1] = true;
				graph->adj[v2-1][v1-1] = true;
			}
		}
		//Adj initialization ends

		int degree = 0;
		int maxDegree = -INF;
		for(int i=0;i<(graph->numVertices);i++){
			degree = 0;
			for(int j=0;j<(graph->numVertices);j++){
				degree += graph->adj[i][j];
			}

			if(degree > maxDegree)
				maxDegree = degree;
		}

		graph->maxDegree = maxDegree;
		//Graph initialization ends
	}

	//Displays graph related info
	void displayGraph(Graph graph){
		printf("Known Chromatic Num: %d\n",graph.knownChromaticNum);
		printf("Number of vertices: %d\n",graph.numVertices);
		printf("Number of Edges: %d\n",graph.numEdges);
		printf("Max Degree: %d\n",graph.maxDegree);
		
		printf("Adj matrix:\n");
		for(int i=0;i<graph.numVertices;i++){
			for(int j=0;j<graph.numVertices;j++){
				printf("%d ",graph.adj[i][j]);
			}
			printf("\n");
		}
	}
/*
	//buildEdgeMatrix takes the file as input and builds the edge matrix corresponding to the graph.
	//Make sure to read the first line of the file and then call this function.
	void buildOriginalEdgeMatrix( FILE *file, int edges[][2], int *numEdges, int numVertices){
		int v1,v2;
		int edgeIndex=0;
		
		bool presentEdges[numVertices][numVertices];

		for(int i=0;i<numVertices;i++){
			for(int j=0;j<numVertices;j++){
				presentEdges[i][j]=false;
			}
		}

		while((fscanf(file,"%*s %d %d",&v1,&v2))==2){
			if(!presentEdges[v1-1][v2-1]){
				edges[edgeIndex][0]=v1-1;
				edges[edgeIndex][1]=v2-1;

				presentEdges[v1-1][v2-1]=true;
				presentEdges[v2-1][v1-1]=true;

				edgeIndex++;
			}
		}

		*numEdges=edgeIndex;

		return ;
	}

	//getGraphInfo gets the known chromatic number, number of vertices, number of edges and the edges information
	void getGraphInfo( FILE *file, int *knownChromaticNum, int *numVertices, int *numEdges, int *numCompEdges, int edges[][2], int compEdges[][2]){
		//Build the original graph
		fscanf(file,"%d %d %d",knownChromaticNum,numVertices,numEdges);
		buildOriginalEdgeMatrix(file,edges,numEdges,*numVertices);

		//Begin building the complement graph
		bool presentCompEdges[*numVertices][*numVertices];

		//Initiate the compADJ matrix with 1s except in the major diagonal i.e.,
		//create the complete graph of order |V| x |V|
		for(int i=0;i<*numVertices;i++){
			for(int j=0;j<*numVertices;j++){
				presentCompEdges[i][j] = (i!=j);
			}
		}

		//For each edge present in the original graph delete that edge from
		//the complete graph to get the complement.
		for(int i=0;i<*numEdges;i++){
			presentCompEdges[edges[i][0]][edges[i][1]] = false;
		}
		
		//To get the edge matrix of the complement graph iterate through the upper half of the compADJ matrix
		// and insert the edge wherever there is a 1 in the compADJ matrix
		int compEdgeIndex = 0;

		for(int i=0;i<*numVertices;i++){
			for(int j=0;j<*numVertices;j++){
				if(j>i && presentCompEdges[i][j]){
					compEdges[compEdgeIndex][0] = i;
					compEdges[compEdgeIndex][1] = j;
					
					compEdgeIndex++;
				}
			}
		}

		*numCompEdges = compEdgeIndex;

		return ;
	}

	void displayGraph(int edges[][2],int numEdges,int numVertices,int knownChromaticNum){
		printf("Known Chromatic Number: %d\n",knownChromaticNum);
		printf("Number of vertices: %d\n",numVertices);
		printf("Number of edges: %d\n",numEdges);

		for(int i=0;i<numEdges;i++){
			printf("%d %d\n",edges[i][0],edges[i][1]);
		}

		return ;
	}

*/

#endif
