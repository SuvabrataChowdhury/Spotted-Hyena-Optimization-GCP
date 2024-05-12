#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define INF 99999

#include"graph.h"
#include"agents.h"

#define MAX_ITR 1
#define NUM_AGENTS 10
#define COLOR_WEIGHT 0.25
#define CONFLICT_WEIGHT 0.75

#define H_MAX 2.0

clock_t start,end;

void SHO_GCP(Graph graph,int maxItr,int numAgents,double conflictWeight,double colorWeight,int maxPos){
	//Start Timer
	start = clock();

	//Main block of algorithm
	//Initialization phase
	Agent preHuntAgents[numAgents];
	Agent postHuntAgents[numAgents];

	//Initialize the preHuntAgents with random agents
	getRandomAgents(graph,preHuntAgents,numAgents,maxPos-1,conflictWeight,colorWeight);
	printf("PreHuntAgents:\n");
	displayAgents(preHuntAgents,numAgents);

	//Allocate memory for position of postHuntAgents which will be used for comparison purpose
	for(int i=0;i<numAgents;i++){
		postHuntAgents[i].position = (int *)calloc(graph.numVertices,sizeof(int));
		copyAgent(&preHuntAgents[i],&postHuntAgents[i]);
	}

	printf("PostHuntAgents:\n");
	displayAgents(postHuntAgents,numAgents);

	double h = H_MAX;
	//End of initialization

	int prey,randAgent,target;
	double random;
	//Hunt begins
	for(int i=0;i<maxItr;i++){
		//Info for current iteration
		//Locate prey
		prey = locatePrey(postHuntAgents,numAgents);
		
		//Locate a random agent which is not prey
		randAgent = rand()%numAgents;
		while(randAgent==prey)
			randAgent = rand()%numAgents;
		
		//Select the target.
		//P(target = prey) = 0.5
		//P(target = any agent other than prey) = 0.5/(numAgents-1);
		random = ((double)rand())/RAND_MAX;
		if(random<0.50){
			target = prey;
		}
		else{
			target = randAgent;
		}
		//Info collected

		//Encirclation begins
		//Update the value of H
		h = H_MAX - ((double)(H_MAX * i))/maxItr;
		for(int j=0;j<numAgents;j++){
			if(j!=target){
				encircle(postHuntAgents[target],postHuntAgents[j],h);
			}
		}
		//Encirclation ends

		for(int j=0;j<numAgents;j++){
			postHuntAgents[j].cVal = getCVal(graph,postHuntAgents[j].position);
			postHuntAgents[j].tVal = getTVal(graph,postHuntAgents[j].position);
			postHuntAgents[j].fitness = getFitness(postHuntAgents[j],CONFLICT_WEIGHT,COLOR_WEIGHT);
		}
		
		printf("After Iteration\n");
		displayAgents(postHuntAgents,numAgents);
	}

	//End Timer
	end = clock();

	printf("\nTime taken: %lf\n",((double)(end-start))/((double)CLOCKS_PER_SEC));
}

void main(int argc, char *argv[]){
	srand((unsigned)time(NULL));

	if(argc<2){	//If argument count is less than 2 then
		printf("Please Provide the file name\n");
		exit(1);
	}
	
	//If the git repo is correctly cloned the graphs must be within the same directory.
	char filePath[100]="TEST_DATASET/";
	//char filePath[100]="GCP_DATASET/";
	strcat(filePath,argv[1]);	//Hence the relative file is "GCP_DATASET/"+argv[1]

	FILE *file;
	file=fopen(filePath,"r");	//Open the file in read mode
	
	if(file==NULL){
		printf("Can not open file\n");

		exit(1);
	}
	
	//File has successfully been opened
	//Get graph related informations
	Graph graph;

	getGraphInfo(file,&graph);	
	displayGraph(graph);
	
	fclose(file);
	//File Closed

	//Search for optimal coloration begins
	SHO_GCP(graph,MAX_ITR,NUM_AGENTS,CONFLICT_WEIGHT,COLOR_WEIGHT,graph.knownChromaticNum);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
