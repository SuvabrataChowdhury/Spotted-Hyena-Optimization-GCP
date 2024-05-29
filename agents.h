#ifndef __AGENTS__
	#define __AGENTS__

	//Solution representation
	typedef struct Agent{
		int dimension; //Dimension of solution vector i.e., |V|
		int *solution; //The coloration/partition

		int totalColors; //Total number of distinct colors/partitions/buckets used
	}Agent;

	void swap(int *var1,int *var2){
		int temp = *var1;
		*var1 = *var2;
		*var2 = temp;
	}

	void printArr(int arr[],int len){
		for(int i=0;i<len;i++){
			printf("%d ",arr[i]);
		}
		printf("\n");
	}

	void seqColor(Agent agent,int vertexSeq[],Graph graph){
		//Initiate all three sets
		int *set1 = (int *)calloc(graph.numVertices,sizeof(int));
		int *set2 = (int *)calloc(graph.numVertices,sizeof(int));
		int *set3 = (int *)calloc(graph.numVertices,sizeof(int));
		
		//Set pointers
		int ptr1 = 0;
		int ptr2 = 0;
		int ptr3 = 0;

		bool *colored = (bool *)calloc(graph.numVertices,sizeof(bool)); //lookup table to keep track of which vertices are colored
		bool canRemain = true; //Used to check if a vertex can remain in set1

		int color = 0; //Color/bucket tracker

		//For each vertices in the given sequence do
		for(int i=0;i<graph.numVertices;i++){

			//If vertex is not colored then
			if(!colored[vertexSeq[i]]){
				//Bring the vetex in set1
				set1[ptr1++] = vertexSeq[i];

				//For all other vertices appearing in sequence after the current one do
				for(int j=i+1;j<graph.numVertices;j++){
					//If vertex is not colored
					if(!colored[vertexSeq[j]]){
						//If it is adjacent then
						if(graph.adj[vertexSeq[i]][vertexSeq[j]])
							set2[ptr2++] = vertexSeq[j]; //bring it into set2
						else
							set3[ptr3++] = vertexSeq[j]; //bring it into set3
					}
				}

				//As set3 contains all vertices that are not adjacent to vertexSeq[i]
				//they can be remain in the same bucket.
				//For each vertices in set3 do
				for(int j=0;j<ptr3;j++){
					canRemain = true;

					//For each vertices in set1 check
					for(int k=0;k<ptr1;k++){
						//if vertex in set3 is adjacent to vertex in set1
						if(graph.adj[set3[j]][set1[k]]){

							//then vertex in set3 cannot remain in set1 
							canRemain = false;
							
							break;
						}
					}

					//If the vertex in set3 is not adjacent to any vertex in set1 then
					if(canRemain)
						set1[ptr1++] = set3[j]; //bring it to set1
				}
				
				//Set1 now contains vertices which can belong in the same bucket/color set/partition
				for(int j=0;j<ptr1;j++){
					colored[set1[j]] = true;
					
					//Bring the vertex in the same color set
					agent.solution[set1[j]] = color;
				}

				//Bring out the next color set/bucket/partition
				color++;

				//Empty all the sets
				ptr1 = 0;
				ptr2 = 0;
				ptr3 = 0;
			}
		}
	}

	//Shuffle uses Fisher-Yates Shuffle to generate
	//a permutation of given n items
	void shuffle(int items[],int numItems){
		int randItem = 0;

		//For each item upto the second last item do
		for(int i=0;i<numItems-1;i++){
			//Choose a random index in range [i,numItems-1]
			randItem = rand()%(numItems-i) + i;

			swap(&items[i],&items[randItem]); //Swap the current item with randomly chosen item
		}
	}

	int getTotalColor(int coloration[],int length){
		bool *colors = (bool *)calloc(length,sizeof(int));

		for(int i=0;i<length;i++){
			colors[coloration[i]] = true;
		}

		int totalColors = 0;
		for(int i=0;i<length;i++){
			totalColors = totalColors + colors[i];
		}

		return totalColors;
	}

	void getInitialPopulation(Agent agents[], int numAgents, Graph graph){
		//Get the vertex set
		int *vertices = (int *)calloc(graph.numVertices,sizeof(int));
		for(int i=0;i<graph.numVertices;i++){
			vertices[i] = i;
		}

		//Perform the sequential coloration of graph with a random permutation of the vertex set.
		for(int i=0;i<numAgents;i++){
			agents[i].dimension = graph.numVertices;
			agents[i].solution = (int *)calloc(graph.numVertices,sizeof(int));

			seqColor(agents[i],vertices,graph);
			
			agents[i].totalColors = getTotalColor(agents[i].solution,agents[i].dimension);

			//Shuffle the vertex set to get a new permutation
			shuffle(vertices,graph.numVertices);
		}
	}

	void displayAgents(Agent agents[],int numAgents,Graph graph){
		printf("#Agent\tColoration\tTotal Colors\n");

		for(int i=0;i<numAgents;i++){
			printf("%d\t",i);

			for(int j=0;j<agents[i].dimension;j++){
				printf("%d ",agents[i].solution[j]);
			}
			
			if(agents[i].totalColors==graph.knownChromaticNum)
				printf("\t%d ***\n",agents[i].totalColors); //Denotes chromatic coloring
			else
				printf("\t%d\n",agents[i].totalColors);
		}
	}
#endif
