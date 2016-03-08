#include "fitness.h"
#include "individual.h"

fitness::fitness(){
	
}

//Fitness testing function
void fitness::testFitness(vector<individual*> generation){

	int fitness;
	vector<int> genom;

	for(int i = 0; i < generation.size(); i++){
		fitness = 0;
		genom = generation[i]->getGenomcode();
		for(int j = 0; j < genom.size(); j++){
			if(genom[j] == solutionGenom[j])
				fitness++;
		}
		generation[i]->setFitness(fitness);
	}	
}

//Set the variables used in the fitness class
void fitness::setFitnessVariables(int genomSize){
	/*
	for(int i = 0; i < genomSize; i++){
		solutionGenom.push_back(1);
	}
	*/
	
	
	
	for(int i = 0; i < genomSize; i++){
		solutionGenom.push_back(rand()%2);
	}
	
	
}