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

#endif
