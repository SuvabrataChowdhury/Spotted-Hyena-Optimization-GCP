#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define INF 99999

#include"graph.h"
#include"agents.h"

#define MAX_ITR 1000
#define NUM_AGENTS 100
#define COLOR_WEIGHT 0.35
#define CONFLICT_WEIGHT (1-COLOR_WEIGHT)

void SHO_GCP(int edges[][2],int numEdges,int numVertices,int maxItr,int numAgents,int maxColor,Agent* solution){
	//Initialize the agents
	Agent agents[numAgents];
	getRandomAgents(agents,numAgents,numVertices,maxColor,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
	//printAgents(agents,numAgents);
	
	//Initialize the cluster as a null vector in the nth dimension
	double *cluster = (double *)calloc(numVertices,sizeof(double));

	//Locate prey i.e., the best solution in the agents list
	int prey = locatePrey(agents,numAgents);

	double h = 5.0;
	double avgFitness = getAvgFitness(agents,numAgents);
	double sdFitness = getStandardDeviationFitness(agents,numAgents,avgFitness);
	printf("SD = %lf\n",sdFitness);
	
	int clusterSize = 0;

	//The hunt begins..
	printf("Iteration,Fitness,AVG Fitness,Conflicts,Total Color\n");
	printf("0,%lf,%lf,%d,%d\n",agents[prey].fitness,avgFitness,agents[prey].conflicts,agents[prey].totalColor);
	for(int i=1;i<=maxItr;i++){
		
		//Add prey to the cluster
		clusterSize = getCluster(cluster,numVertices,agents,numAgents,avgFitness,sdFitness);

		//Chase the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey)
				moveToCentroid(agents[j],cluster,agents[j].dimension,(double)clusterSize);
		}

		//Calculate the distance from prey
		for(int j=0;j<numAgents;j++){
			setDistance(agents[j],agents[prey]);
		}

		//Encircle the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey)
				encircle(agents[j],agents[prey],h);
		}
		
		//Update fitness of all agents
		for(int j=0;j<numAgents;j++){
			agents[j].conflicts = getConflicts(agents[j],edges,numEdges);
			agents[j].totalColor = getTotalColor(agents[j]);
			agents[j].fitness = getFitness(agents[j],numVertices,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
		}

		prey = locatePrey(agents,numAgents);
		
		h =  5.0-((5.0*i)/maxItr);
		
		/*
		printf("\nIteration no: %d\n",i);
		printAgents(agents,numAgents);

		printf("\nThe prey is:\n");
		*/
		
		//printAgent(agents[prey]);

		for(int i=0;i<numVertices;i++){
			cluster[i] = 0;
		}

		clusterSize = 0;

		avgFitness = getAvgFitness(agents,numAgents);
		sdFitness = getStandardDeviationFitness(agents,numAgents,avgFitness);

		printf("%d,%lf,%lf,%d,%d\n",i,agents[prey].fitness,avgFitness,agents[prey].conflicts,agents[prey].totalColor);
	}

	//solution = &agents[prey];
}

void main(int argc, char *argv[]){
	srand((unsigned)time(NULL));

	if(argc<2){	//If argument count is less than 2 then
		printf("Please Provide the file name\n");
		exit(1);
	}
	
	//If the git repo is correctly cloned the graphs must be within the same directory.
	char filePath[100]="GCP_DATASET/";
	strcat(filePath,argv[1]);	//Hence the relative file is "GCP_DATASET/"+argv[1]

	FILE *file;
	file=fopen(filePath,"r");	//Open the file in read mode
	
	if(file==NULL){
		printf("Can not open file\n");

		exit(1);
	}
	
	//File has successfully been opened
	
	int knownChromaticNum, numVertices, numEdges;
	int edges[INF][2];
	
	getGraphInfo(file,&knownChromaticNum,&numVertices,&numEdges,edges);
	//displayGraph(edges,numEdges,numVertices,knownChromaticNum);
	Agent solution;
	SHO_GCP(edges,numEdges,numVertices,MAX_ITR,NUM_AGENTS,knownChromaticNum,&solution);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
