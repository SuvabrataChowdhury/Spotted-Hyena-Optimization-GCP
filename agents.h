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
