#ifndef __AGENTS__
	#define __AGENTS__

	//Here a vector means a quantity which has both magnitude and direction.
	//A vector is represented by a tuple consisting of numDimension number of vectors along each principle axis.

	//The search agent structure is as follows,
	typedef struct Agent{
		//Geometric Information
		int dimension; //Defines in which dimension the agent lies
		double *position; //Defines the position vector of the agent.
		double *distFromPrey; //Defines the distance vector of the current agent from the prey i.e., the best solution.
		
		//Solution Information
		int conflicts;
		int totalColor;
		double fitness; //fitness_i := (COLOR_WEIGHT * (num_vertices-total_color)) + (CONFLICT_WEIGHT * (num_edges-conflict))
	}Agent;

	void swap(int *num1, int *num2){
		int temp = *num1;
		*num1 = *num2;
		*num2 = temp;
	}

	//Position of each agent defines a coloration of the graph
	//The translation from a position component to a vertex color can be obtained by,
	//	color_i := round(|position_i|)
	int getColor(double pos){
		return (int)round(fabs(pos));
	}

	int getConflicts(Agent agent,int edges[][2],int numEdges){
		int numConflict = 0;
		//For each edge do
		for(int i=0;i<numEdges;i++){
			//If color of two adjacent vertex is same then increment numConflict
			if( getColor(agent.position[edges[i][0]]) == getColor(agent.position[edges[i][1]]))
				numConflict++;
		}

		return numConflict;
	}

	int getMax(double arr[],int len){
		int max = getColor(arr[0]);

		for(int i=1;i<len;i++){
			if(getColor(arr[i])>max)
				max = getColor(arr[i]);
		}
		
		return max;
	}

	//totalColor is the total number of unique colors used to color the graph
	int getTotalColor(Agent agent){
		int max = getMax(agent.position,agent.dimension);
		bool *table = (bool*) calloc(max+1,sizeof(bool));
		
		//store in the table
		for(int i=0;i<agent.dimension;i++){
			table[getColor(agent.position[i])] = true;
		}

		int totalColor = 0;
		for(int i=0;i<max+1;i++){
			if(table[i])
				totalColor++;
		}

		return totalColor;
	}

	double getFitness(Agent agent,int numVertices,int numEdges,double colorWeight,double conflictWeight){
		double fitness = (colorWeight * (numVertices - agent.totalColor)) + (conflictWeight * (numEdges - agent.conflicts)) ;
		return fitness;
	}

	//Generates the initial configuration of agents
	void getRandomAgents(Agent agents[],int numAgents,int numVertices,int maxPos,int edges[][2],int numEdges,double colorWeight,double conflictWeight){
		for(int i=0;i<numAgents;i++){
			//Set the dimension of each vector as number of vertices of the graph
			agents[i].dimension = numVertices;
			//The position is a vector which defines position of each agent
			agents[i].position = (double*) calloc(numVertices,sizeof(double));
			
			//Place the agent in a random position bounded by maxPos
			for(int j=0;j<agents[i].dimension;j++){
				agents[i].position[j] = (((double)rand())/RAND_MAX)*(maxPos-1);
			}

			//Initiate distFromPrey vector
			agents[i].distFromPrey = (double*) calloc(numVertices,sizeof(double));

			//Get the solution related informations
			//Initiate conflicts
			agents[i].conflicts = getConflicts(agents[i],edges,numEdges);
			//Initiate totalColor
			agents[i].totalColor = getTotalColor(agents[i]);
			//Initiate fitness
			agents[i].fitness = getFitness(agents[i],numVertices,numEdges,colorWeight,conflictWeight);
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

	int locatePrey(int topAgents[],int numTopAgents){
		return topAgents[0];
	}

	/*
	//Prey is the agent having maximum fitness
	int locatePrey(Agent agents[],int numAgents){
		int prey = 0;
		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness > agents[prey].fitness)
				prey = i;
		}

		return prey;
	}
	*/

	void addVectors(double vec1[],double vec2[],int len){
		for(int i=0;i<len;i++){
			vec1[i] = vec1[i] + vec2[i];
		}
	}

	void moveToCentroid(Agent agent,double vec[],int len,double scaleFactor){
		for(int i=0;i<len;i++){
			agent.position[i] = vec[i]/scaleFactor;
		}
	}

	//D_h = |B * P_p - P_h|
	//B = 2 * rd_1
	//rd_1 belongs to [0,1]
	void setDistance(Agent agent, Agent prey){
		double randComponent = 0.0;
		for(int i=0;i<agent.dimension;i++){
			randComponent = (1.0*rand())/RAND_MAX;
			agent.distFromPrey[i] = fabs( (2 * randComponent * prey.position[i]) - agent.position[i]);
		}
	}

	//P_h(x+1) = P_p - E * D_h
	//E = 2 * rd_2 * h - h
	//rd_2 belongs to [0,1]
	void encircle(Agent hyena, Agent prey, double h, double maxPos){
		double randComponent = 0.0;
		double randScaleComponent = 0.0;

		for(int i=0;i<hyena.dimension;i++){
			randComponent = (1.0*rand())/RAND_MAX;
			randScaleComponent = (2.0*h*randComponent) - h;
			//To be modified the portal logic...
			hyena.position[i] = prey.position[i] - (randScaleComponent * hyena.distFromPrey[i]);
		}
	}

	void printArr(double arr[],int len){
		for(int i=0;i<len;i++){
			printf("%lf ",arr[i]);
		}
		printf("\n");
	}

	double getAvgFitness(Agent agents[],int numAgents){
		double sum = 0.0;

		for(int i=0;i<numAgents;i++){
			sum = sum + agents[i].fitness;
		}

		return sum/numAgents;
	}
	
	bool belongsIn(int item,bool table[],int tableLength){
		return item<tableLength && table[item];
	}
	
	//Stores top n agents in topAgents
	//topAgents actually holds the indices of each best agents
	void getTopAgents(Agent agents[],int numAgents,int topAgents[],int numTopAgents){
		int lastPos = numTopAgents-1;

		for(int i=0;i<numAgents;i++){
			if(agents[i].fitness > agents[topAgents[lastPos]].fitness){
				topAgents[lastPos] = i;

				for(int j=lastPos;j>0;j--){
					if(agents[topAgents[j-1]].fitness < agents[topAgents[j]].fitness){
						swap(&topAgents[j],&topAgents[j-1]);
					}
					else
						break;
				}
			}
		}
	}
	
	void getCluster(double cluster[], bool clusterTable[], int clusterLength, Agent agents[], int numAgents, int topAgents[], int numTopAgents){
		for(int i=1;i<numTopAgents;i++){
			clusterTable[topAgents[i]] = true;

			addVectors(cluster,agents[i].position,clusterLength);
		}
	}
/*
	// Battle Royale Selection procedure
	int getCluster(double cluster[], bool clusterTable[], int clusterLength, Agent agents[], int numAgents, int prey, double thresholdFitness, double sdFitness){
		//Initially cluster is empty
		int clusterSize = 0;

		//Select atleast clusterSize number of hyenas
		while(clusterSize<2){
			//For each agents do
			for(int i=0;i<numAgents;i++){
				//If agent is not prey and agent has not been selected and agent's fitness exceeds the threshold fitness then
				if(i!=prey && !belongsIn(i,clusterTable,clusterLength) && agents[i].fitness>=thresholdFitness){
					//Include the agent/hyena in the cluster
					//Include it in cluster table
					clusterTable[i] = true;
					
					//Include it in cluster
					addVectors(cluster,agents[i].position,clusterLength);
					
					//Increment the number of selected hyenas
					clusterSize++;
				}
			}

			//If less than 2 hyenas have been selected then decrement the threshold fitness by sd to allow selection for new hyenas
			thresholdFitness -= sdFitness;
		}

		//printf("%d %lf\n",selectedHyenas,thresholdFitness);

		//Return the cluster size
		return clusterSize;
	}
*/
/*
	//Tournament Selection procedure
	int getCluster(double cluster[], int clusterLength, Agent agents[], int numAgents, int prey){
		int clusterSize = numAgents;

		for(int i=0;i<numAgents;i++){
			int rand1 = rand()%numAgents;
			int rand2 = rand()%numAgents;
			
			while(rand1==rand2){
				rand2 = rand()%numAgents;
			}

			if(agents[rand1].fitness > agents[rand2].fitness){
				addVectors(cluster,agents[rand1].position,clusterLength);
			}
			else{
				addVectors(cluster,agents[rand2].position,clusterLength);
			}
		}

		return clusterSize;
	}
*/
	/*
		getCluster:
			Input: The agents list and a null cluster vector.
			Output:	Constructed cluster with n best solutions and n
			[Cluster gets created with roulette wheel selection method]
	*/
/*	int getCluster( double cluster[], bool clusterTable[],int clusterLength, Agent agents[], int numAgents, int prey){
		double sumFitness = 0.0;

		for(int i=0;i<numAgents;i++){
			sumFitness = sumFitness + agents[i].fitness;
		}

		double randSum = 0.0;
		double currSum = 0.0;

		int index = 0;
		int clusterSize = 0;
		for(int i=0;i<numAgents;i++){
			randSum = ((1.0*rand())/RAND_MAX) * sumFitness;
			
			currSum = 0.0;
			index = 0;
			while( (index < numAgents) && (currSum < randSum) ){
				currSum = currSum + agents[index].fitness;
				index++;
			}

			if( (currSum > randSum) && (index < numAgents) && index!=prey && !belongsIn(index,clusterTable)){
				clusterTable[index] = true;
				addVectors(cluster,agents[index].position,clusterLength);
				clusterSize++;
			}
		}

		return clusterSize;
	}
*/
/*
	int getCluster(double cluster[], int clusterLength, Agent agents[], int numAgents, double avgFitness, double sdFitness, int prey){
		int clusterSize = 0;
		//For each agent
		for(int i=0;i<numAgents;i++){
			if(i!=prey){
				//If the agent is better than the average agent then add it to the cluster
				if( (agents[i].fitness - avgFitness)/(sdFitness+EPS) >= 1.0){
					addVectors(cluster,agents[i].position,clusterLength);
					clusterSize = clusterSize + 1;
				}
			}
		}

		return clusterSize;
	}
*/
	double getSDFitness(Agent agents[], int numAgents, double avgFitness){
		double sumSquareDev = 0.0;

		for(int i=0;i<numAgents;i++){
			sumSquareDev += pow((avgFitness - agents[i].fitness),2);
		}

		return sqrt(sumSquareDev/numAgents);
	}

#endif
