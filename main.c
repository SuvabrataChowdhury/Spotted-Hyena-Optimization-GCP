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

clock_t start,end;

void SHO_GCP(Graph graph,int numAgents,int maxItr){
	Agent agents[numAgents]; //The population
	int* result = (int*)calloc(graph.numVertices,sizeof(int));

	getInitialPopulation(agents,numAgents,graph,result);

	int prey = getPrey(agents,numAgents);
	
	//double avgColors = getAvgPartitions(agents,numAgents);

	printf("Iteration,Min Colors\n");
	printf("0,%d\n",agents[prey].partitions);
	//printf("0,%d,%lf\n",agents[prey].partitions,avgColors);

	//The hunt begins..
	for(int i=1;i<=maxItr;i++){
		//If prey's partitions is <= knownChromatic number then stop the hunt
		if(agents[prey].partitions <= graph.knownChromaticNum)
			break;
		
		//For each hyenas do
		for(int j=0;j<numAgents;j++){
			if(j!=prey){
				//Surround the prey..
				encircle(agents[prey],agents[j],((double)i)/maxItr);
				
				//Find the partitions as hyena's position has changed
				agents[j].partitions = getPartitions(agents[j],graph,result,false);
			}
		}

		//Find the prey as prey may be changed due to encirclation
		prey = getPrey(agents,numAgents);
		//avgColors = getAvgPartitions(agents,numAgents);

		//printf("%d,%d,%lf\n",i,agents[prey].partitions,avgColors);
		printf("%d,%d\n",i,agents[prey].partitions);
	}

	printf("\nObtained Coloration:\n");
	getPartitions(agents[prey],graph,result,true);
	printArr(result,graph.numVertices);

	printf("Obtained Chromatic number: %d\n",agents[prey].partitions);
	printf("Known Chromatic number: %d\n",graph.knownChromaticNum);

	if(agents[prey].partitions <= graph.knownChromaticNum)
		printf("Chromatic Coloration found\n");
	else
		printf("Chromatic Coloration not found\n");
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
	start = clock();
	SHO_GCP(graph,NUM_AGENTS,MAX_ITR);
	end = clock();

	printf("Time Taken = %lf\n",(double)(end-start)/CLOCKS_PER_SEC);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
