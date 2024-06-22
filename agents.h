#ifndef __AGENTS__
	#define __AGENTS__

	//Solution representation
	typedef struct Agent{
		int dimension; //Dimension of solution vector i.e., |V|
		
		int *vertexSeq; //Vertex permutation for finding optimal coloration
		int *coloration; //Coloration obtained from vertexSeq

		int partitions; //Total number of partitions/buckets used
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

	int getPartitions(Agent agent,Graph graph){
		int numPartitions = 0;

		int *partition = (int *)calloc(graph.numVertices,sizeof(int));
		int ptr = 0;

		bool *colored = (bool *)calloc(graph.numVertices,sizeof(int));

		int color = 0;
		bool canBePlaced = true;

		//For each vertices do
		for(int i=0;i<graph.numVertices;i++){
			//If vertex has not been colored then
			if(!colored[agent.vertexSeq[i]]){
				//Put it into the partition
				partition[ptr++] = agent.vertexSeq[i];
				
				//For remaining vertices do
				for(int j=i+1;j<graph.numVertices;j++){
					//If those are not colored then
					if(!colored[agent.vertexSeq[j]]){
						canBePlaced = true;
						
						//For those already in partition
						for(int k=0;k<ptr;k++){
							//if an adjacency exists then both of them can not be in the same partition
							if(graph.adj[partition[k]][agent.vertexSeq[j]]){
								canBePlaced = false;
								break;
							}
						}
						
						//If both of them can be placed in the same partition then
						if(canBePlaced){
							partition[ptr++] = agent.vertexSeq[j]; //append the vertex in the same partition
							colored[agent.vertexSeq[j]] = true;
						}
					}
				}

				//Assign colors to each vertex
				//(This block can be removed as the actual coloration is not required... )
				for(int j=0;j<ptr;j++){
					agent.coloration[partition[j]] = color;
				}

				color++;
			}

			//Empty out partition
			ptr = 0;
		}

		return color;
	}

	void getInitialPopulation(Agent agents[], int numAgents, Graph graph){
		//Get the vertex set
		agents[0].dimension = graph.numVertices;
		agents[0].vertexSeq = (int *)calloc(graph.numVertices,sizeof(int));
		agents[0].coloration = (int *)calloc(graph.numVertices,sizeof(int));

		for(int i=0;i<graph.numVertices;i++){
			agents[0].vertexSeq[i] = i;
		}
		
		agents[0].partitions = getPartitions(agents[0],graph);

		//Perform the sequential coloration of graph with a random permutation of the vertex set.
		for(int i=1;i<numAgents;i++){
			agents[i].dimension = graph.numVertices;
			agents[i].vertexSeq = (int *)calloc(graph.numVertices,sizeof(int));
			agents[i].coloration = (int *)calloc(graph.numVertices,sizeof(int));

			for(int j=0;j<graph.numVertices;j++){
				agents[i].vertexSeq[j] = j;
			}

			//Shuffle the vertex set to get a new permutation
			shuffle(agents[i].vertexSeq,graph.numVertices);

			agents[i].partitions = getPartitions(agents[i],graph);
		}
	}

	void displayAgents(Agent agents[],int numAgents,Graph graph){
		printf("#Agent\tVertex Sequence\tColoration\tTotal Colors\n");

		for(int i=0;i<numAgents;i++){
			printf("%d\t",i);

			for(int j=0;j<agents[i].dimension;j++){
				printf("%d ",agents[i].vertexSeq[j]);
			}

			printf("\t");
			for(int j=0;j<agents[i].dimension;j++){
				printf("%d ",agents[i].coloration[j]);
			}
			
			if(agents[i].partitions==graph.knownChromaticNum)
				printf("\t%d ***\n",agents[i].partitions); //Denotes chromatic coloring
			else
				printf("\t%d\n",agents[i].partitions);
		}
	}
#endif
