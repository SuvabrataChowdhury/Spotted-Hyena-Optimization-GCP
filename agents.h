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
	// Max value for cVal is |V|
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

			if(degree!=0)
				cVal += (1.0-(((double)conflictCount)/degree));
		}

		return cVal;
	}

	//tVal is a mesure of how many vertices share colors.
	// tVal = sum for each vertex in V' (1 - con_i'/deg_i')
	// here,
	// con_i' is the number of conflicting neighbours of ith vertex in the complement graph and
	// deg_i' is the degree of ith vertex in the complement graph
	// Max value of tVal is |V'|
	double getTVal(Graph graph,int coloration[]){
		double tVal = 0.0;
		int compConflictCount = 0;
		int compDegree = 0;

		for(int i=0;i<graph.numVertices;i++){
			compConflictCount = 0;
			compDegree = 0;

			for(int j=0;j<graph.numVertices;j++){
				if(i!=j){
					compDegree += !graph.adj[i][j];

					if(!graph.adj[i][j] && coloration[i]==coloration[j]){
						compConflictCount++;
					}
				}
			}

			if(compDegree!=0){
				tVal = tVal + ((1.0*compConflictCount)/compDegree);
			}
		}

		return tVal;
	}

	//Fitness gives more weightage to reducing the conflict
	double getFitness(Agent agent,double conflictWeight,double colorWeight){
		return conflictWeight * agent.cVal + colorWeight * agent.tVal;
	}

	int getConflicts(Graph graph,Agent agent){
		int conflicts = 0;

		for(int i=0;i<graph.numVertices;i++){
			for(int j=0;j<graph.numVertices;j++){
				if(i<j && graph.adj[i][j]==1 && agent.position[i]==agent.position[j]){
					conflicts++;
				}
			}
		}

		return conflicts;
	}
	
	//getTotalColor finds the total number of unique colors used by the given agent.
	//The function assumes that atmost numVertices number of color is used.
	int getTotalColor(Agent agent){
		bool *table = (bool *)calloc(agent.dimension,sizeof(bool));

		for(int i=0;i<agent.dimension;i++){
			table[agent.position[i]] = true;
		}

		int totalColor = 0;

		for(int i=0;i<agent.dimension;i++){
			totalColor += table[i];
		}

		return totalColor;
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

	//Deep copies agent info from source to destination
	void copyAgent(Agent *source,Agent *dest){
		dest->dimension = source->dimension;

		for(int i=0;i<(source->dimension);i++){
			dest->position[i] = source->position[i];
		}

		dest->cVal = source->cVal;
		dest->tVal = source->tVal;
		dest->fitness = source->fitness;
	}

	//Prey is the agent having maximum fitness
	int locatePrey(Agent agents[],int numAgents){
		int prey = 0;

		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness>agents[prey].fitness){
				prey = i;
			}
		}

		return prey;
	}

	//bound bounds the solutions in the positive quadrant
	double bound(double var,double maxPos){
		if(var < 0.0)
			return 0.0;
		else if(var > maxPos)
			return maxPos;
		else
			return var;
	}

	//addVectors adds two vectors vec1 and vec2 and stores the result in vec1
	void addVectors(int vec1[],int vec2[],int dimension){
		for(int i=0;i<dimension;i++){
			vec1[i] = vec1[i] + vec2[i];
		}
	}

	void getCluster(Graph graph,Agent agents[],int numAgents,int target,int maxPos,int centroid[],double conflictWeight,double colorWeight){
		//First select the suitable region in the search space
		//Initialize the dummyAgent
		Agent dummyAgent;
		dummyAgent.dimension = agents[target].dimension;
		dummyAgent.position = (int*) calloc(dummyAgent.dimension,sizeof(int));

		//Get a dummyAgent which is close to the target
		int trFactors[] = {0,0,1,-1};
		int numTrFactors = sizeof(trFactors)/sizeof(trFactors[0]);
		for(int i=0;i<dummyAgent.dimension;i++){
			dummyAgent.position[i] = bound(agents[target].position[i] + trFactors[rand()%numTrFactors],maxPos);
		}

		//Find the fitness of such agent
		dummyAgent.cVal = getCVal(graph,dummyAgent.position);
		dummyAgent.tVal = getTVal(graph,dummyAgent.position);
		dummyAgent.fitness = getFitness(dummyAgent,conflictWeight,colorWeight);
		//End of initialization of the dummyAgent

		printf("DummyAgent info,\n");
		printf("Dimension: %d\n",dummyAgent.dimension);
		printf("Position:\n");
		for(int i=0;i<dummyAgent.dimension;i++){
			printf("%d ",dummyAgent.position[i]);
		}
		printf("\nC Value: %lf\n",dummyAgent.cVal);
		printf("T Value: %lf\n",dummyAgent.tVal);
		printf("Fitness: %lf\n",dummyAgent.fitness);

		//Begin constructing the cluster
		int clusterSize;

		double maxFitness = (dummyAgent.fitness>agents[target].fitness)?dummyAgent.fitness:agents[target].fitness;
		double minFitness = (dummyAgent.fitness<agents[target].fitness)?dummyAgent.fitness:agents[target].fitness;
		printf("\nSelection Range: %lf to %lf\n",minFitness,maxFitness);
		for(int i=0;i<numAgents;i++){
			//If the agent's fitness lies within the range then include it in the cluster
			if(i!=target && (agents[i].fitness>(minFitness-EPSILON) && agents[i].fitness<(maxFitness+EPSILON))){
				printf("Agent[%d] selected\n",i);
				addVectors(centroid,agents[i].position,agents[i].dimension);
				clusterSize++;
			}
		}

		//If dummyAgent has exceeded the target in fitness then include target in the cluster and replace target with dummyAgent,
		if(dummyAgent.fitness>agents[target].fitness){
			printf("Dummy Exceeded target\n");
			addVectors(centroid,agents[target].position,agents[target].dimension);
			copyAgent(&dummyAgent,&agents[target]);
		}
		else{
		//else, include dummyAgent in cluster let the target retain its position 
			printf("Target remains same\n");
			addVectors(centroid,dummyAgent.position,agents[target].dimension);
		}
		clusterSize++;
		//cluster construction ends

		//Centroid construction begins
		printf("Centroid:\n");
		for(int i=0;i<agents[target].dimension;i++){
			centroid[i] = (int)round(((double)centroid[i])/clusterSize);
			printf("%d ",centroid[i]);
		}
		printf("\n");
		//Centroid construction ends
	}

	/*
	double uniformRandom(int levels){
		int range = rand()%levels;

		double diffRandom = (((double)rand())/RAND_MAX)*(1.0/levels);

		return diffRandom + (((double)range)/levels);
	}
	*/

	//encircle uses the equations to move the hyena towards or away from prey
	void encircle(Agent prey,Agent hyena,double h,double maxPos){
		double dist = 0.0;
		double vecB = 0.0;
		double vecE = 0.0;
		double hyenaPos = 0.0;

		for(int i=0;i<prey.dimension;i++){
			vecB = (2.0 * rand())/RAND_MAX;
			dist = fabs(vecB * prey.position[i] - hyena.position[i]);
			
			vecE = ((2.0 * h * rand())/RAND_MAX) - h;

			hyenaPos = prey.position[i] - vecE * dist;
			hyena.position[i] = (int)round(bound(hyenaPos,maxPos));
		}
	}

	double findAvgFitness(Agent agents[],int numAgents){
		double sumFitness = 0.0;

		for(int i=0;i<numAgents;i++){
			sumFitness += agents[i].fitness;
		}

		return sumFitness/numAgents;
	}
#endif
