#ifndef __AGENTS__
	#define __AGENTS__

	#define EPSILON 0.000001

	typedef struct Agent{
		//Solution itself
		int dimension;
		int *position;

		//Score of the solution i.e., fitness
		double cVal; //Fuzzy non-conflicted vertex count
		double tVal; //Fuzzy color share count [to be implemented...]

		double fitness; //Actual score based on cVal and tVal
	}Agent;

	//cVal is a fuzzy count of non-Conflicted vertices
	// cVal = sum for each vertex in V (1 - con_i/deg_i)
	// here,
	// con_i is the number of conflicting neighbours of ith vertex and
	// deg_i is the degree of ith vertex
	double getCVal(Graph graph,int coloration[]){
		double cVal = 0.0;
		int conflictCount = 0;
		int degree = 0;

		for(int i=0;i<graph.numVertices;i++){
			conflictCount = 0;
			degree = 0;

			for(int j=0;j<graph.numVertices;j++){
				degree += graph.adj[i][j];

				if(graph.adj[i][j] && coloration[i]==coloration[j]){
					conflictCount++;
				}
			}

			cVal += (1.0-(((double)conflictCount)/degree));
		}

		return cVal;
	}

	double getTVal(Graph graph,int coloration[]){
		return 0.0; //For now, to be changed later
	}

	//Fitness gives more weightage to reducing the conflict
	double getFitness(Agent agent,double conflictWeight,double colorWeight){
		return conflictWeight * agent.cVal + colorWeight * agent.tVal;
	}

	//Initializes a random population
	void getRandomAgents(Graph graph,Agent agents[],int numAgents,int maxPos,double conflictWeight,double colorWeight){
		//For each agents do
		for(int i=0;i<numAgents;i++){
			//Initialize dimension
			agents[i].dimension = graph.numVertices;

			//Initialize the position vector which is the solution itself..
			agents[i].position = (int *)calloc(graph.numVertices,sizeof(int));
			for(int j=0;j<graph.numVertices;j++){
				agents[i].position[j] = random()%(maxPos+1);
			}

			agents[i].cVal = getCVal(graph,agents[i].position);
			agents[i].tVal = getTVal(graph,agents[i].position);
			
			agents[i].fitness = getFitness(agents[i],conflictWeight,colorWeight);
		}
	}

	void displayAgents(Agent agents[],int numAgents){
		printf("Position\tcVal\ttVal\tfitness\n");
		for(int i=0;i<numAgents;i++){
			for(int j=0;j<agents[i].dimension;j++){
				printf("%d ",agents[i].position[j]);
			}
			printf("\t%lf\t%lf\t%lf\n",agents[i].cVal,agents[i].tVal,agents[i].fitness);
		}
	}
#endif
