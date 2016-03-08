#include "individual.h"
#include "genotypes.h"
#include "phenotypes.h"


individual::individual(int genomSize){
	genotype = new genotypes(genomSize);
	phenotype = new phenotypes();	
}

//Constructor for individuals that inherit its genomcode
individual::individual(vector<int> genomCode){
	genotype = new genotypes(genomCode);
	phenotype = new phenotypes();
}

individual::~individual(){
	delete genotype;
	delete phenotype;
}

//Return the genomCode
vector<int> individual::getGenomcode(){
	return genotype->returnGenom();
}

//Return an individuals fitness value
int individual::getFitness(){
	return fitness;
}

//Mutate a genom by m percent
void individual::mutateGenom(float mutationPercentage){
	genotype->mutate(mutationPercentage);
}

void individual::setFitness(int fitnessScore){
	fitness = fitnessScore;
}

void individual::evolve(){
	delete phenotype;
	phenotype = new phenotypes(genotype->returnGenom());
}