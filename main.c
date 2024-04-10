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
#define COLOR_WEIGHT 0.1
#define CONFLICT_WEIGHT (1-COLOR_WEIGHT)

void SHO_GCP(int edges[][2],int numEdges,int numVertices,int maxItr,int numAgents,int maxColor,Agent* solution){
	//Initialize the agents
	Agent agents[numAgents];
	getRandomAgents(agents,numAgents,numVertices,maxColor,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
	
	//printAgents(agents,numAgents);
	
	//Initialize the cluster as a null vector in the nth dimension
	double *circCentroid = (double *)calloc(numVertices,sizeof(double));
	bool *clusterTable = (bool *)calloc(NUM_AGENTS,sizeof(bool));

	double h = 5.0;
	double avgFitness = getAvgFitness(agents,numAgents);
	double sdFitness = getSDFitness(agents,numAgents,avgFitness);
	
	int clusterSize = 0;

	//Locate prey i.e., the best solution in the agents list
	int prey = locatePrey(agents,numAgents);
	int bestHyena = locateBestHyena(agents,numAgents,prey);
	int worstHyena = locateWorstHyena(agents,numAgents);

	//The hunt begins..
	printf("Iteration,Fitness,AVG Fitness,Standard Deviation,Cluster Size,Conflicts,Total Color\n");
	printf("0,%lf,%lf,%lf,0,%d,%d\n",agents[prey].fitness,avgFitness,sdFitness,agents[prey].conflicts,agents[prey].totalColor);

	for(int i=1;i<=maxItr;i++){
		clusterSize = getCluster(agents,numAgents,circCentroid,clusterTable,bestHyena,worstHyena,maxColor-1,numVertices,edges,numEdges,COLOR_WEIGHT,CONFLICT_WEIGHT);
		//printf("ClusterSize: %d\n",clusterSize);

		//Chase the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey && !belongsIn(j,clusterTable,NUM_AGENTS))
				moveToCentroid(agents[j],circCentroid,agents[j].dimension);
		}

		//Calculate the distance from prey
		for(int j=0;j<numAgents;j++){
			setDistance(agents[j],agents[prey]);
		}

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
		
		h =  5.0-((5.0*i)/maxItr);

		//Empty the cluster for next iteration
		for(int j=0;j<numVertices;j++){
			circCentroid[j] = 0.0;
		}

		for(int j=0;j<numAgents;j++){
			clusterTable[j] = false;
		}

		avgFitness = getAvgFitness(agents,numAgents);
		sdFitness = getSDFitness(agents,numAgents,avgFitness);

		printf("%d,%lf,%lf,%lf,%d,%d,%d\n",i,agents[prey].fitness,avgFitness,sdFitness,clusterSize,agents[prey].conflicts,agents[prey].totalColor);

		prey = locatePrey(agents,numAgents);
		bestHyena = locateBestHyena(agents,numAgents,prey);
		worstHyena = locateWorstHyena(agents,numAgents);
		//printAgents(agents,numAgents);
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
