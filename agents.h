#ifndef __AGENTS__
	#define __AGENTS__

	#define COLOR_WEIGHT 0.65
	#define CONFLICT_WEIGHT (1-COLOR_WEIGHT)

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
			if(((int)round(fabs(agent.position[edges[i][0]]))) == ((int)round(fabs(agent.position[edges[i][1]]))))
				numConflict++;
		}

		return numConflict;
	}

	double getMax(double arr[],int len){
		double max = fabs(arr[0]);

		for(int i=1;i<len;i++){
			if(fabs(arr[i])>max)
				max = fabs(arr[i]);
		}
		
		return max;
	}

	int getTotalColor(Agent agent){
		double max = getMax(agent.position,agent.dimension);
		int tableLength = (int)round(max+1);
		bool *table = (bool*) calloc(tableLength,sizeof(bool));
		
		//store in the table
		for(int i=0;i<agent.dimension;i++){
			table[(int)round(fabs(agent.position[i]))] = true;
		}

		int totalColor = 0;
		for(int i=0;i<tableLength;i++){
			if(table[i])
				totalColor++;
		}

		return totalColor;
	}

	double getFitness(Agent agent,int numVertices,int numEdges){
		double fitness = (COLOR_WEIGHT * (numVertices - agent.totalColor)) + (CONFLICT_WEIGHT * (numEdges - agent.conflicts)) ;
		return fitness;
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
			agents[i].fitness = getFitness(agents[i],numVertices,numEdges);
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

	int locatePrey(Agent agents[],int numAgents){
		int prey = 0;
		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness > agents[prey].fitness)
				prey = i;
		}

		return prey;
	}

	void addVectors(double vec1[],double vec2[],int len){
		for(int i=0;i<len;i++){
			vec1[i] = vec1[i] + vec2[i];
		}
	}

	void setVector(double vec1[],double vec2[],int len,double scaleFactor){
		for(int i=0;i<len;i++){
			vec1[i] = vec2[i]/scaleFactor;
		}
	}

	void setDistance(Agent agent, Agent prey){
		double randComponent = 0.0;
		for(int i=0;i<agent.dimension;i++){
			randComponent = (1.0*rand())/RAND_MAX;
			agent.distFromPrey[i] = fabs( (2 * randComponent * prey.position[i]) - agent.position[i]);
		}
	}

	void encircle(Agent hyena, Agent prey, double h){
		double randComponent = 0.0;
		double randScaleComponent = 0.0;

		for(int i=0;i<hyena.dimension;i++){
			randComponent = (1.0*rand())/RAND_MAX;
			randScaleComponent = (2.0*h*randComponent) - h;
			hyena.position[i] = prey.position[i] - (randScaleComponent * hyena.distFromPrey[i]);
		}
	}

	void printArr(double arr[],int len){
		for(int i=0;i<len;i++){
			printf("%lf ",arr[i]);
		}
		printf("\n");
	}

	void SHO_GCP(int edges[][2],int numEdges,int numVertices,int maxItr,int numAgents,int maxColor,Agent* solution){
		//Initialize the agents
		Agent agents[numAgents];
		getRandomAgents(agents,numAgents,numVertices,maxColor,edges,numEdges);
		//printAgents(agents,numAgents);
		
		//Initialize the cluster as a null vector in the nth dimension
		double *cluster = (double *)calloc(numVertices,sizeof(double));

		//Locate prey i.e., the best solution in the agents list
		int prey = locatePrey(agents,numAgents);

		/*
		printf("\nThe prey is:\n");
		printAgent(agents[prey]);
		*/

		double h = 5.0;
		//The hunt begins..
		printf("Iteration,Fitness,Conflicts,Total Color\n");
		printf("0,%lf,%d,%d\n",agents[prey].fitness,agents[prey].conflicts,agents[prey].totalColor);
		for(int i=1;i<=maxItr;i++){
			//Add prey to the cluster
			addVectors(cluster,agents[prey].position,agents[prey].dimension);

			//Chase the pray
			for(int j=0;j<numAgents;j++){
				setVector(agents[j].position,cluster,agents[j].dimension,(double)i);
			}

			prey = 0;

			//Calculate the distance from prey
			for(int j=0;j<numAgents;j++){
				setDistance(agents[j],agents[prey]);
			}

			//Encircle the prey
			for(int j=0;j<numAgents;j++){
				encircle(agents[j],agents[prey],h);
			}
			
			//Update fitness of all agents
			for(int j=0;j<numAgents;j++){
				agents[j].conflicts = getConflicts(agents[j],edges,numEdges);
				agents[j].totalColor = getTotalColor(agents[j]);
				agents[j].fitness = getFitness(agents[j],numVertices,numEdges);
			}

			prey = locatePrey(agents,numAgents);
			
			h =  5.0-((5.0*i)/maxItr);
			
			/*
			printf("\nIteration no: %d\n",i);
			printAgents(agents,numAgents);

			printf("\nThe prey is:\n");
			*/
			
			//printAgent(agents[prey]);

			printf("%d,%lf,%d,%d\n",i,agents[prey].fitness,agents[prey].conflicts,agents[prey].totalColor);
		}

		//solution = &agents[prey];
	}
#endif