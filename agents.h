#ifndef __AGENTS__
	#define __AGENTS__

	#define PRECISION 5

	//Here a vector means a quantity which has both magnitude and direction.
	//A vector is represented by a tuple consisting of numDimension number of vectors along each principle axis.

	//The search agent structure is as follows,
	typedef struct Agent{
		int dimension; //Defines in which dimension the agent lies
		double *position; //Defines the position vector of the agent.
		double *distFromPrey; //Defines the distance vector of the current agent from the prey i.e., the best solution.
		int conflicts;
		int totalColor;
		double fitness; //Defines how good this agent is.
	}Agent;

	int getConflicts(Agent agent,int edges[][2],int numEdges){
		int numConflict = 0;
		for(int i=0;i<numEdges;i++){
			if(round(agent.position[edges[i][0]]) == round(agent.position[edges[i][1]]))
				numConflict++;
		}

		return numConflict;
	}

	double getMax(double arr[],int len){
		double max = (-1.0) * INF;

		for(int i=0;i<len;i++){
			if(arr[i]>max)
				max = arr[i];
		}
		
		printf("Max = %lf\n",max);
		return max;
	}

	int getTotalColor(Agent agent){
		double max = getMax(agent.position,agent.dimension);
		int tableLength = (int)round(max+1);
		bool *table = (bool*) calloc(tableLength,sizeof(bool));
		
		//store in the table
		for(int i=0;i<agent.dimension;i++){
			printf("%lf ",round(agent.position[i]));
			table[(int)round(agent.position[i])] = true;
		}
		printf("\n");
		for(int i=0;i<tableLength;i++)
			printf("%d ",table[i]);
		printf("\n");
		
		int totalColor = 0;
		for(int i=0;i<tableLength;i++){
			if(table[i])
				totalColor++;
		}

		return totalColor;
	}

	double getFitness(Agent agent){
		return (double)agent.conflicts;
	}

	//Generates the initial configuration of agents
	void getRandomAgents(Agent agents[],int numAgents,int numVertices,int maxPos,int edges[][2],int numEdges){
		for(int i=0;i<numAgents;i++){
			agents[i].dimension = numVertices;
			agents[i].position = (double*) calloc(numVertices,sizeof(double));
			
			for(int j=0;j<agents[i].dimension;j++){
				agents[i].position[j] = ((rand()*(1.0))/RAND_MAX)*maxPos;
			}

			agents[i].distFromPrey = (double*) calloc(numVertices,sizeof(double));

			agents[i].conflicts = getConflicts(agents[i],edges,numEdges);
			agents[i].totalColor = getTotalColor(agents[i]);
			agents[i].fitness = getFitness(agents[i]);
		}
	}

	void printAgent(Agent agent){
		printf("Dimension = %d\n",agent.dimension);
		
		printf("Position = ");
		for(int i=0;i<agent.dimension;i++){
			printf("%lf ",agent.position[i]);
		}
		printf("\n");

		printf("Distance From Prey = ");
		for(int i=0;i<agent.dimension;i++){
			printf("%lf ",agent.distFromPrey[i]);
		}
		printf("\n");

		printf("Conflicts = %d\n",agent.conflicts);
		printf("Total Color = %d\n",agent.totalColor);
		printf("Fitness = %lf\n",agent.fitness);
	}

	void printAgents(Agent agents[],int numAgents){
		for(int i=0;i<numAgents;i++){
			printf("Agent[%d]:\n",i);
			printAgent(agents[i]);
		}

		return ;
	}

	void SHO_GCP(int edges[][2],int numEdges,int numVertices,int maxItr,int numAgents,int maxColor,Agent* solution){
		
		Agent agents[numAgents];
		getRandomAgents(agents,numAgents,numVertices,maxColor,edges,numEdges);
		printAgents(agents,numAgents);
	}
#endif
