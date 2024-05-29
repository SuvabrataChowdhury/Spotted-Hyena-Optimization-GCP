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

clock_t start,end;

void SHO_GCP(Graph graph,int numAgents){
	Agent agents[numAgents]; //The population

	getInitialPopulation(agents,numAgents,graph);
	displayAgents(agents,numAgents,graph);
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
	SHO_GCP(graph,NUM_AGENTS);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
