#ifndef __AGENTS__
	#define __AGENTS__

	#define EPSILON 0.000001

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

	//As in a dimension where numbers are arranged in a circular way one can join two points with two vectors,
	//one going in clockwise direction and another going in the anticlockwise direction, we need the CircDirection datatype
	//to convay such motion
	typedef enum CircDirection {CLOCK,ANTICLOCK} CircDirection;

	void swap(int *num1, int *num2){
		int temp = *num1;
		*num1 = *num2;
		*num2 = temp;
	}

	void fswap(double *num1, double *num2){
		double temp = *num1;
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

	double max(double num1, double num2){
		return (num1>num2)?num1:num2;
	}

	double min(double num1, double num2){
		return (num1<num2)?num1:num2;
	}

	int getMaxColor(double arr[],int len){
		int max = getColor(arr[0]);

		for(int i=1;i<len;i++){
			if(getColor(arr[i])>max)
				max = getColor(arr[i]);
		}
		
		return max;
	}

	//totalColor is the total number of unique colors used to color the graph
	int getTotalColor(Agent agent){
		int max = getMaxColor(agent.position,agent.dimension);
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

	void copyAgent(Agent *source,Agent *dest){
		dest->dimension = source->dimension;

		for(int i=0;i<(dest->dimension);i++){
			dest->position[i] = source->position[i];
		}
		
		dest->conflicts = source->conflicts;
		dest->totalColor = source->totalColor;
		dest->fitness = source->fitness;
	}
/*	
	//Prey is the agent having maximum fitness
	//BestHyena is the agent having second highest fitness
	void locatePreyAndBestHyena(Agent agents[],int numAgents,int *prey,int *bestHyena){
		*prey = 0;
		*bestHyena = 0;
		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness > agents[*prey].fitness)
				*prey = i;

			if(agents[i].fitness < agents[*prey].fitness && agents[i].fitness > agents[*bestHyena].fitness)
				*bestHyena = i;
		}
	}
*/
	//Prey is the agent having maximum fitness
	int locatePrey(Agent agents[],int numAgents){
		int prey = 0;
		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness > agents[prey].fitness)
				prey = i;
		}

		return prey;
	}

	//Prey is the agent having maximum fitness
	int locateBestHyena(Agent agents[],int numAgents,int prey){
		int bestHyena = 0;
		for(int i=1;i<numAgents;i++){
			if(i!=prey && agents[i].fitness > agents[bestHyena].fitness)
				bestHyena = i;
		}

		return bestHyena;
	}

	//Prey is the agent having maximum fitness
	int locateWorstHyena(Agent agents[],int numAgents){
		int worstHyena = 0;
		for(int i=1;i<numAgents;i++){
			if(agents[i].fitness < agents[worstHyena].fitness)
				worstHyena = i;
		}

		return worstHyena;
	}

	void addVectors(double vec1[],double vec2[],int len){
		for(int i=0;i<len;i++){
			vec1[i] = vec1[i] + vec2[i];
		}
	}
	
	void moveToCentroid(Agent agent,double centroid[],int numDimension){
		for(int i=0;i<numDimension;i++){
			agent.position[i] = centroid[i];
		}
	}

/*
	void moveToCentroid(Agent agent,double vec[],int len,double scaleFactor){
		for(int i=0;i<len;i++){
			agent.position[i] = vec[i]/scaleFactor;
		}
	}
*/

	//Calculate the circular distance between pos1 and pos2 in the given direction bounded by maxPos
	double circDist(double pos1, double pos2, double maxPos, CircDirection direction){
		return (direction == ANTICLOCK)? fmod(pos2 + (maxPos-pos1),maxPos) : fmod(pos1 + (maxPos-pos2),maxPos);
	}

	//Finds the center of the arc between pos1 and pos2 in the given direction bounded by maxPos
	double circAvg(double pos1, double pos2, double maxPos, CircDirection direction){
		double startPoint = (direction == ANTICLOCK) ? pos1 : pos2;

		return fmod((startPoint + 0.5 * circDist(pos1,pos2,maxPos,direction)),maxPos);
	}

	bool fequals(double val1,double val2){
		return fabs(val1-val2) < EPSILON;
	}

	bool isBetween(double pos, double startPos, double endPos, double maxPos, CircDirection direction){
		return fequals(circDist(startPos,pos,maxPos,direction) + circDist(pos,endPos,maxPos,direction),circDist(startPos,endPos,maxPos,direction));
	}

	CircDirection comp(CircDirection direction){
		return (direction == ANTICLOCK)?CLOCK:ANTICLOCK;
	}

	//bound bounds the given variable with maxPos in such a way that if var goes out of the boundary then
	//it comes right back around from the other direction 
	double bound(double var, double maxPos){
		if( var < 0 ){
			return maxPos-fmod(fabs(var),maxPos);
		}
		else{
			return fmod(var,maxPos);
		}
	}

	//D_h = |B * P_p - P_h|
	//B = 2 * rd_1
	//rd_1 belongs to [0,1]
	void setDistance(Agent agent, Agent prey, int maxPos){
		double randComponent = 0.0;
		for(int i=0;i<agent.dimension;i++){
			randComponent = (1.0*rand())/RAND_MAX;
			//agent.distFromPrey[i] = fabs( (2 * randComponent * prey.position[i]) - agent.position[i]);
			agent.distFromPrey[i] = circDist(agent.position[i],(2 * randComponent * prey.position[i]),maxPos,ANTICLOCK);
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
			//hyena.position[i] = prey.position[i] - (randScaleComponent * hyena.distFromPrey[i]);

			hyena.position[i] = bound(prey.position[i] - (randScaleComponent * hyena.distFromPrey[i]) ,maxPos);
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


	//Finds the toroidal center component in the given dimension with the given list of vectors.
	//This center is chosen to be the center of the arc consisting of the most increasing regions of
	//the fitness function.
	double toricCenter(Agent agents[], int numAgents, int incrOrderAgents[],int numIncrAgents,double maxPos,int dimension){
		double startPoint = agents[incrOrderAgents[0]].position[dimension];
		double endPoint = agents[incrOrderAgents[0]].position[dimension];
		
		double target = agents[incrOrderAgents[numIncrAgents-1]].position[dimension];
		
		CircDirection direction = ANTICLOCK;

		for(int i=1;i<numIncrAgents-1;i++){
			if( !isBetween(agents[incrOrderAgents[i]].position[dimension],startPoint,target,maxPos,direction) ){
				direction = comp(direction);
				fswap(&startPoint,&endPoint);
			}

			if( !isBetween(agents[incrOrderAgents[i]].position[dimension],startPoint,endPoint,maxPos,direction) ){
				endPoint = agents[incrOrderAgents[i]].position[dimension];
			}
		}

		endPoint = target;

		return circAvg(startPoint,endPoint,maxPos,direction);
	}

	//Cluster formation using nearest neighbours based on fitness on a toric world
	int getCluster(Agent agents[], int numAgents, double circCentroid[], bool clusterTable[], int bestHyena, int worstHyena, int maxPos,int dimension, int edges[][2], int numEdges, double colorWeight, double conflictWeight){

		//Create a dummyHyena for the purpose of finding the gradient of the fitness function locally
		Agent dummyHyena;

		dummyHyena.dimension = dimension;
		dummyHyena.position = (double *) calloc(dimension,sizeof(double));
		dummyHyena.distFromPrey = (double *) calloc(dimension,sizeof(double));

		//To find the gradient first place the dummyHyena by translating the bestHyena with a very small factor( in range [0.5,1] )
		for(int i=0;i<dimension;i++){
			dummyHyena.position[i] = bound( (agents[bestHyena].position[i] + ((0.5*rand())/RAND_MAX) + 0.5) ,maxPos);
		}

		dummyHyena.conflicts = getConflicts(dummyHyena,edges,numEdges);
		dummyHyena.totalColor = getTotalColor(dummyHyena);
		dummyHyena.fitness = getFitness(dummyHyena,dimension,numEdges,colorWeight,conflictWeight);
		
		//Add the best hyena in the cluster,
		int clusterSize = 1;

		clusterTable[bestHyena] = true;

		//If gradient is positive then we might be near a maxima (either global or local)
		//then explore that region
		if(dummyHyena.fitness - agents[bestHyena].fitness > 0){
			//replace the worst agent with dummyHyena
			copyAgent(&dummyHyena,&agents[worstHyena]);

			//Add the newly added dummyHyena to the cluster
			clusterSize++;

			clusterTable[worstHyena] = true;

			for(int i=0;i<dimension;i++){
				circCentroid[i] = circAvg(agents[bestHyena].position[i],agents[worstHyena].position[i],maxPos,ANTICLOCK);
			}
		}
		else{
			int numBestAgents = 15;
			int *bestAgentsInRange = (int*) calloc(numBestAgents,sizeof(int));

			//Initialize the bestAgentsInRange with the worstAgent
			for(int i=0;i<numBestAgents;i++){
				bestAgentsInRange[i] = worstHyena;
			}

			bestAgentsInRange[0] = bestHyena;

			//Construct the priority queue with top n best agents bounded by bestHyena and worstAgent
			int lastIndex = 1;
			for(int i=0;i<numAgents;i++){
				if((agents[i].fitness >= dummyHyena.fitness && agents[i].fitness <= agents[bestHyena].fitness) && (agents[bestAgentsInRange[lastIndex]].fitness < agents[i].fitness)){

					bestAgentsInRange[lastIndex] = i;

					for(int j=lastIndex;j>0;j--){
						if(agents[bestAgentsInRange[j-1]].fitness < agents[bestAgentsInRange[j]].fitness){
							swap(&bestAgentsInRange[j-1],&bestAgentsInRange[j]);
						}
					}

					if(lastIndex<numBestAgents-1)
						lastIndex++;
				}
			}

			int actualLastIndex = (lastIndex==numBestAgents-1) ? lastIndex : (lastIndex-1);
			
			clusterSize = actualLastIndex + 1;

			int *bestAgents = (int*) calloc(actualLastIndex+1,sizeof(int));

			for(int i=actualLastIndex;i>=0;i--){
				bestAgents[actualLastIndex - i] = bestAgentsInRange[i];
			}
			
			for(int i=0;i<dimension;i++){
				circCentroid[i] = toricCenter(agents,numAgents,bestAgents,clusterSize,maxPos,i);
			}
		}

		return clusterSize;
	}
/*	
	//Finds distance between two numbers arranged in a circular way
	double circDist(double pos1, double pos2, double maxPos){
		return fmod(pos2 + (maxPos-pos1),maxPos);
	}

	//Finds circular distance between two vectors where primary axis are not straight lines but
	//they loop around after maxPos units
	void circDistVec(double vec1[],double vec2[],double result[],double maxPos,int numDimension){
		for(int i=0;i<numDimension;i++){
			result[i] = circDist(vec1[i],vec2[i],maxPos);
		}
	}

	//Scales a vector uniformly in each dimension with the given scaleFactor
	void uniformScale(double vec[],double numDimension,double scaleFactor){
		for(int i=0;i<numDimension;i++){
			vec[i] = vec[i] * scaleFactor;
		}
	}

	//Cluster formation using nearest neighbours based on fitness on a toric world
	int getCluster(int edges[][2], int numVertices, int numEdges, double colorWeight, double conflictWeight, double circCentroid[], bool clusterTable[], Agent agents[], int numAgents, int prey, int bestHyena, int worstHyena, double maxPos){
		//Initialize the dummyHyena which defines the range of solutions to be selected with
		//the best hyena
		Agent dummyHyena;

		dummyHyena.dimension = numVertices;
		dummyHyena.position = (double *)calloc(numVertices,sizeof(double));
		dummyHyena.distFromPrey = (double *)calloc(numVertices,sizeof(double));

		//Translate the dummyHyena from the best hyena's position with a random vector having components in [0.5,1]
		for(int i=0;i<numVertices;i++){
			dummyHyena.position[i] = bound(agents[bestHyena].position[i]+(((0.5*rand())/RAND_MAX) + 0.5),maxPos);
		}

		dummyHyena.conflicts = getConflicts(dummyHyena,edges,numEdges);
		dummyHyena.totalColor = getTotalColor(dummyHyena);
		dummyHyena.fitness = getFitness(dummyHyena,numVertices,numEdges,colorWeight,conflictWeight);

		//Add the best hyena in the cluster,
		int clusterSize = 0;

		clusterTable[bestHyena] = true;

		addVectors(circCentroid,agents[bestHyena].position,numVertices);

		double *circDist = (double *)calloc(numVertices,sizeof(double));

		//If gradient is positive then we might be near a maxima (either global or local)
		//then explore that region
		if(dummyHyena.fitness - agents[bestHyena].fitness > 0){
			//replace the worst agent with dummyHyena
			copyAgent(&dummyHyena,&agents[worstHyena]);

			//Add the newly added dummyHyena to the cluster
			clusterSize = 2;

			clusterTable[worstHyena] = true;

			circDistVec(agents[worstHyena].position,agents[bestHyena].position,circDist,maxPos,numVertices);
			
			uniformScale(circDist,numVertices,0.5);

			addVectors(circCentroid,circDist,numVertices);
		}
		else{
			int numBestAgents = 10;
			int *bestAgentsInRange = (int*) calloc(numBestAgents,sizeof(int));

			//Initialize the bestAgentsInRange with the worstAgent
			for(int i=0;i<numBestAgents;i++){
				bestAgentsInRange[i] = worstHyena;
			}

			bestAgentsInRange[0] = bestHyena;

			//Construct the priority queue with top n best agents bounded by bestHyena and worstAgent
			int lastIndex = 1;
			for(int i=0;i<numAgents;i++){
				if((agents[i].fitness >= dummyHyena.fitness && agents[i].fitness <= agents[bestHyena].fitness) && (agents[bestAgentsInRange[lastIndex]].fitness < agents[i].fitness)){

					bestAgentsInRange[lastIndex] = i;

					for(int j=lastIndex;j>0;j--){
						if(agents[bestAgentsInRange[j-1]].fitness < agents[bestAgentsInRange[j]].fitness){
							swap(&bestAgentsInRange[j-1],&bestAgentsInRange[j]);
						}
					}

					if(lastIndex<numBestAgents-1)
						lastIndex++;
				}
			}

			int actualLastIndex = (lastIndex==numBestAgents-1) ? lastIndex : (lastIndex-1);
			
			clusterSize = actualLastIndex + 1;

			//For each agents do
			for(int i=actualLastIndex;i>0;i--){
				//include it in the cluster

				clusterTable[bestAgentsInRange[i]] = true;

				circDistVec(agents[bestAgentsInRange[i]].position,agents[bestAgentsInRange[i-1]].position,circDist,maxPos,numVertices);
				
				uniformScale(circDist,numVertices,0.5);

				addVectors(circCentroid,circDist,numVertices);
			}
		}

		for(int i=0;i<numVertices;i++){
			circCentroid[i] = fmod(circCentroid[i],maxPos);
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
