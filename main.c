#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define INF 99999

#include"graph.h"
#include"agents.h"

#define MAX_ITR 5
#define NUM_AGENTS 10
#define CONFLICT_WEIGHT 0.75
#define COLOR_WEIGHT (1.0-CONFLICT_WEIGHT)

#define H_MAX 5.0

clock_t start,end;

void SHO_GCP(Graph graph,int maxItr,int numAgents,double conflictWeight,double colorWeight,int maxColors){
	//Start Timer
	start = clock();

	//Main block of algorithm
	//Initialization phase
	Agent preHuntAgents[numAgents];
	Agent postHuntAgents[numAgents];

	//Initialize the preHuntAgents with random agents
	getRandomAgents(graph,preHuntAgents,numAgents,maxColors-1,conflictWeight,colorWeight);
	displayAgents(preHuntAgents,numAgents);

	//Allocate memory for position of postHuntAgents which will be used for comparison purpose
	for(int i=0;i<numAgents;i++){
		postHuntAgents[i].position = (int *)calloc(graph.numVertices,sizeof(int));
		copyAgent(&preHuntAgents[i],&postHuntAgents[i]);
	}

	double h = H_MAX;

	int *centroid = (int *)calloc(graph.numVertices,sizeof(int));
	//End of initialization

	int prey,randAgent,target;
	int preyTotalColor,preyConflicts;
	double random;
	double avgFitness = findAvgFitness(preHuntAgents,numAgents);

	int numImproved = 0;

	//Hunt begins
	//printf("Iteration,Fitness,cVal,tVal,Conflicts,Total Color,AVG fitness\n");

	prey = locatePrey(preHuntAgents,numAgents);
	preyConflicts = getConflicts(graph,preHuntAgents[prey]);
	preyTotalColor = getTotalColor(preHuntAgents[prey]);
	avgFitness = findAvgFitness(preHuntAgents,numAgents);
	//printf("%d,%lf,%lf,%lf,%d,%d,%lf\n",0,postHuntAgents[prey].fitness,postHuntAgents[prey].cVal,postHuntAgents[prey].tVal,preyConflicts,preyTotalColor,avgFitness);
	for(int i=0;i<maxItr;i++){
		//Break Condition
		if(((int)preHuntAgents[prey].cVal)==graph.numVertices && preyTotalColor<=graph.knownChromaticNum)
			break;

		//Info for current iteration
		//Locate a random agent which is not prey
		randAgent = rand()%numAgents;
		while(numAgents!=1 && randAgent==prey){
			randAgent = rand()%numAgents;
		}
		
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
		printf("Target: Agent[%d]\n",target);

		//Encirclation begins
		//Form the cluster about the target
		getCluster(graph,preHuntAgents,numAgents,target,maxColors-1,centroid,conflictWeight,colorWeight);

		for(int j=0;j<numAgents;j++){
			for(int dim=0;dim<graph.numVertices;dim++){
				postHuntAgents[j].position[dim] = centroid[dim];
			}
		}
		//cluster formation ends

		//Update the value of H
		h = H_MAX - ((double)(H_MAX * i))/maxItr;
		for(int j=0;j<numAgents;j++){
			if(j!=target){
				encircle(postHuntAgents[target],postHuntAgents[j],h,(double)maxColors-1.0);
			}
		}
		
		//Get the fitness of each agents after encirclation
		for(int j=0;j<numAgents;j++){
			postHuntAgents[j].cVal = getCVal(graph,postHuntAgents[j].position);
			postHuntAgents[j].tVal = getTVal(graph,postHuntAgents[j].position);
			postHuntAgents[j].fitness = getFitness(postHuntAgents[j],CONFLICT_WEIGHT,COLOR_WEIGHT);
		}
		//Encirclation ends

		//Retention process begins
		for(int j=0;j<numAgents;j++){
			//If after hunting agent improves then
			if(postHuntAgents[j].fitness>preHuntAgents[j].fitness){
				//retain the position
				copyAgent(&postHuntAgents[j],&preHuntAgents[j]);
				numImproved++;
			}
			else{
				//Else, revert back
				copyAgent(&preHuntAgents[j],&postHuntAgents[j]);
			}
		}
		//Retention process ends

		prey = locatePrey(postHuntAgents,numAgents);
		preyConflicts = getConflicts(graph,postHuntAgents[prey]);
		preyTotalColor = getTotalColor(postHuntAgents[prey]);
		avgFitness = findAvgFitness(preHuntAgents,numAgents);
		//printf("%d,%lf,%lf,%lf,%d,%d,%lf\n",i+1,postHuntAgents[prey].fitness,postHuntAgents[prey].cVal,postHuntAgents[prey].tVal,preyConflicts,preyTotalColor,avgFitness);

		//Revert centroid back to a null vector
		for(int j=0;j<graph.numVertices;j++)
			centroid[j] = 0;

		printf("\nIteration[%d]\n",i);
		displayAgents(postHuntAgents,numAgents);
		printf("%d agents improved\n",numImproved);
		numImproved = 0;
	}

	//End Timer
	end = clock();

	printf("\nTime taken: %lf\n",((double)(end-start))/((double)CLOCKS_PER_SEC));
	printf("Best solution found:\n");
	for(int i=0;i<graph.numVertices;i++){
		printf("%d ",preHuntAgents[prey].position[i]);
	}
	printf("\n");
}

void main(int argc, char *argv[]){
	srand(time(0));

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
	//displayGraph(graph);

	fclose(file);
	//File Closed

	//Search for optimal coloration begins
	//SHO_GCP(graph,MAX_ITR,NUM_AGENTS,CONFLICT_WEIGHT,COLOR_WEIGHT,graph.numVertices);
	//SHO_GCP(graph,MAX_ITR,NUM_AGENTS,CONFLICT_WEIGHT,COLOR_WEIGHT,graph.maxDegree+1);
	SHO_GCP(graph,MAX_ITR,NUM_AGENTS,CONFLICT_WEIGHT,COLOR_WEIGHT,graph.knownChromaticNum);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
