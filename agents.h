#ifndef __AGENTS__
	#define __AGENTS__

	//The search agent structure is as follows,
	typedef struct Agent{
		int numDimension; //Defines in which dimension the agent lies
		double *position; //Defines the position vector of the agent.
		double *distFromPrey; //Defines the distance vector of the current agent from the prey i.e., the best solution.
		double fitness; //Defines how good this agent is.
	}Agent;
	//Here a vector means a quantity which has both magnitude and direction.
	//A vector is represented by a tuple consisting of numDimension number of vectors along each principle axis.
#endif
