#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define INF 99999

#include"graph.h"
#include"agents.h"

#define MAX_ITR 10000
#define NUM_AGENTS 100
#define COLOR_WEIGHT 0.05
#define CONFLICT_WEIGHT 0.95

#define H_MAX 2.0

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

	//Allocate memory for position of postHuntAgents which will be used for comparison purpose
	for(int i=0;i<numAgents;i++){
		postHuntAgents[i].position = (int *)calloc(graph.numVertices,sizeof(int));
		copyAgent(&preHuntAgents[i],&postHuntAgents[i]);
	}

	double h = H_MAX;
	//End of initialization

	int prey,randAgent,target;
	int preyTotalColor;
	double random;
	double avgFitness = findAvgFitness(preHuntAgents,numAgents);

	//Hunt begins
	printf("Iteration,Fitness,cVal,tVal,Total Color,AVG fitness\n");

	prey = locatePrey(preHuntAgents,numAgents);
	preyTotalColor = getTotalColor(preHuntAgents[prey]);
	avgFitness = findAvgFitness(preHuntAgents,numAgents);
	printf("%d,%lf,%lf,%lf,%d,%lf\n",0,postHuntAgents[prey].fitness,postHuntAgents[prey].cVal,postHuntAgents[prey].tVal,preyTotalColor,avgFitness);
	for(int i=0;i<MAX_ITR;i++){
		//Break Condition
		if(((int)preHuntAgents[prey].cVal)==graph.numVertices && preyTotalColor<=graph.knownChromaticNum)
			break;

		//Encirclation begins
		//Update vector h
		h = H_MAX - H_MAX*(((double)i)/MAX_ITR);

		//For each agent select it's prey
		for(int j=0;j<numAgents;j++){
			target = rand()%numAgents;
			while(target==j)
				target = rand()%numAgents;

			//After prey selection encircle it
			encircle(preHuntAgents[target],postHuntAgents[j],h,(double)maxColors-1.0);
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
			}
			else{
				//Else, revert back
				copyAgent(&preHuntAgents[j],&postHuntAgents[j]);
			}
		}
		//Retention process ends

		prey = locatePrey(postHuntAgents,numAgents);
		preyTotalColor = getTotalColor(postHuntAgents[prey]);
		avgFitness = findAvgFitness(preHuntAgents,numAgents);
		printf("%d,%lf,%lf,%lf,%d,%lf\n",i+1,postHuntAgents[prey].fitness,postHuntAgents[prey].cVal,postHuntAgents[prey].tVal,preyTotalColor,avgFitness);
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
	//char filePath[100]="TEST_DATASET/";
	char filePath[100]="GCP_DATASET/";
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
