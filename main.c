#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define INF 99999

#include"graph.h"
#include"agents.h"

#define MAX_ITR 100000
#define NUM_AGENTS 100
#define COLOR_WEIGHT 0.35
#define CONFLICT_WEIGHT (1-COLOR_WEIGHT)

#define H_MAX 1.0
#define START_COUNTDOWN 1000

void SHO_GCP(int edges[][2],int numEdges,int numVertices,int maxItr,int numAgents,int maxColor,int knownChromaticNum,Agent* solution){

	//Initialize the agents
	Agent agents[numAgents];
	//getRandomAgents(agents,numAgents,numVertices,maxColor-1,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
	getBiasedAgents(agents,numAgents,numVertices,maxColor-1,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
	
	//printAgents(agents,numAgents);

	//Initialize the cluster as a null vector in the nth dimension
	double *circCentroid = (double *)calloc(numVertices,sizeof(double));
	bool *clusterTable = (bool *)calloc(NUM_AGENTS,sizeof(bool));

	double h = H_MAX;
	double avgFitness = getAvgFitness(agents,numAgents);
	double sdFitness = getSDFitness(agents,numAgents,avgFitness);
	
	int clusterSize = 0;

	//Locate prey i.e., the best solution in the agents list
	int prey,bestHyena,worstHyena;

	//The hunt begins..
	printf("Iteration,Best Fitness,AVG Fitness,Worst Fitness,Prey Conflicts,Prey Total Color,Worst Conflicts,Worst Total Color\n");
	for(int i=1;i<=maxItr;i++){
		prey = locatePrey(agents,numAgents);
		bestHyena = locateBestHyena(agents,numAgents,prey);
		worstHyena = locateWorstHyena(agents,numAgents);

		printf("%d,%lf,%lf,%lf,%d,%d,%d,%d\n",i,agents[prey].fitness,avgFitness,agents[worstHyena].fitness,agents[prey].conflicts,agents[prey].totalColor,agents[worstHyena].conflicts,agents[worstHyena].totalColor);

		clusterSize = getCluster(agents,numAgents,circCentroid,clusterTable,bestHyena,worstHyena,maxColor-1,numVertices,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);

		if(agents[prey].conflicts==0 && agents[prey].totalColor<=knownChromaticNum)
			break;

		//Chase the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey && !belongsIn(j,clusterTable,NUM_AGENTS))
				moveToCentroid(agents[j],circCentroid,agents[j].dimension);
		}

		/*
		if(((double)rand())/RAND_MAX < exp(((-10.0)*i)/(double)maxItr)){
			prey = worstHyena;
		}
		*/

		//Encircle the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey && !belongsIn(j,clusterTable,NUM_AGENTS))
				encircle(agents[j],agents[prey],h,maxColor-1);
		}
		
		//Update fitness of all agents
		for(int j=0;j<numAgents;j++){
			
			if(j!=prey && !belongsIn(j,clusterTable,NUM_AGENTS)){
				agents[j].conflicts = getConflicts(agents[j],edges,numEdges);
				agents[j].totalColor = getTotalColor(agents[j]);
				agents[j].fitness = getFitness(agents[j],numVertices,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
			}
		}
		
		h = H_MAX - (H_MAX*(((double)i)/maxItr));

		//Empty the cluster for next iteration
		for(int j=0;j<numVertices;j++){
			circCentroid[j] = 0.0;
		}

		for(int j=0;j<numAgents;j++){
			clusterTable[j] = false;
		}

		avgFitness = getAvgFitness(agents,numAgents);
		sdFitness = getSDFitness(agents,numAgents,avgFitness);
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
	SHO_GCP(edges,numEdges,numVertices,MAX_ITR,NUM_AGENTS,knownChromaticNum,knownChromaticNum,&solution);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
