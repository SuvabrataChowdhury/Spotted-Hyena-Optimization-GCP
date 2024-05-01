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

#define H_MAX 5.0
#define START_COUNTDOWN 1000

clock_t start,end;

void SHO_GCP(int edges[][2],int numEdges,int compEdges[][2],int numCompEdges,int numVertices,int maxItr,int numAgents,int maxColor,int knownChromaticNum,Agent* solution){
	start = clock();

	//Initialize the agents
	Agent agents[numAgents];
	//getRandomAgents(agents,numAgents,edges,numEdges,compEdges,numCompEdges,numVertices,(double)maxColor-1.0,COLOR_WEIGHT,CONFLICT_WEIGHT);
	getBiasedAgents(agents,numAgents,edges,numEdges,compEdges,numCompEdges,numVertices,(double)maxColor-1.0,COLOR_WEIGHT,CONFLICT_WEIGHT);
	
	//printAgents(agents,numAgents);

	//Initialize the cluster as a null vector in the nth dimension
	double *centroid = (double *)calloc(numVertices,sizeof(double));

	double h = H_MAX;
	
	int clusterSize = 0;

	//Locate prey i.e., the best solution in the agents list
	int prey,worstHyena;

	double avgFitness = 0.0;

	//The hunt begins..
	printf("Iteration,Best Fitness,C Val,T Val,Conflicts,Total Color,AVG fitness,SD fitness\n");
	for(int i=1;i<=maxItr;i++){
		prey = locatePrey(agents,numAgents);
		worstHyena = locateWorstHyena(agents,numAgents);

		avgFitness = getAvgFitness(agents,numAgents);

		printf("%d,%lf,%lf,%lf,%d,%d,%lf\n",i,agents[prey].fitness,agents[prey].cVal,agents[prey].tVal,agents[prey].conflicts,agents[prey].totalColor,avgFitness);

		if(((int)agents[prey].cVal)==numVertices && agents[prey].totalColor<=knownChromaticNum)
			break; //Solution found

		clusterSize = getCluster(agents,numAgents,prey,worstHyena,edges,numEdges,compEdges,numCompEdges,centroid,(double)(maxColor-1),COLOR_WEIGHT,CONFLICT_WEIGHT);
		
		//Chase the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey)
				moveToCentroid(agents[j],centroid,agents[j].dimension);
		}

		//Encircle the prey
		for(int j=0;j<numAgents;j++){
			if(j!=prey)
				encircle(agents[j],agents[prey],h,maxColor-1);
		}
		
		//Update fitness of all agents
		for(int j=0;j<numAgents;j++){
			
			agents[j].conflicts = getConflicts(agents[j],edges,numEdges);
			//Initiate totalColor
			agents[j].totalColor = getTotalColor(agents[j]);
			//Initiate fitness
			agents[j].cVal = getCVal(agents[j],edges,numEdges,(double)maxColor-1);
			agents[j].tVal = getTVal(agents[j],compEdges,numCompEdges,(double)maxColor-1);
			agents[j].fitness = getFitness(agents[j],numEdges,numVertices,COLOR_WEIGHT,CONFLICT_WEIGHT);
		}
		
		h = H_MAX - (H_MAX*(((double)i)/maxItr));
	}
	
	end = clock();

	printf("\nTime taken: %lf\n",((double)(end-start))/((double)CLOCKS_PER_SEC));
	printf("\nThe best coloration obtained: \n");

	for(int i=0;i<numVertices;i++){
		printf("%lf ",agents[prey].position[i]);
	}
	printf("\n");

	//solution = &agents[prey];
}

void main(int argc, char *argv[]){
	srand((unsigned)time(NULL));

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
	
	int knownChromaticNum, numVertices, numEdges, numCompEdges;
	int edges[INF][2],compEdges[INF][2];
	
	getGraphInfo(file,&knownChromaticNum,&numVertices,&numEdges,&numCompEdges,edges,compEdges);

	fclose(file);

	Agent solution;
	SHO_GCP(edges,numEdges,compEdges,numCompEdges,numVertices,MAX_ITR,NUM_AGENTS,numVertices,knownChromaticNum,&solution);
	//SHO_GCP(edges,numEdges,compEdges,numCompEdges,numVertices,MAX_ITR,NUM_AGENTS,knownChromaticNum,knownChromaticNum,&solution);

	//printf("Obtained Solution:\n");
	//printAgent(solution);
	
	return ;
}
