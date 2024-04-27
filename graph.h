#ifndef __GRAPH__
	#define __GRAPH__

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

#endif
