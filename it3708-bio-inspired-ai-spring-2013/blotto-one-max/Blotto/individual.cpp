#include "individual.h"
#include "genotypes.h"
#include "phenotypes.h"

//First cycle child constructor
individual::individual(int genomSize){
	fitness = 0;
	genotype = new genotypes(genomSize);
	phenotype = new phenotypes();
	
}

//Constructor for individuals that inherit its genomcode
individual::individual(vector<int> genomCode){
	fitness = 0;
	genotype = new genotypes(genomCode);
	phenotype = new phenotypes(genomCode);
}

individual::~individual(){
	delete genotype;
	delete phenotype;
}

// Evolve a individual by creating a phenotype from its genotype
void individual::evolve(){ 
	delete phenotype;
	phenotype = new phenotypes(genotype->returnGenom());
}

//Return the genomCode
vector<int> individual::getGenomcode(){
	return genotype->returnGenom();
}

vector<float> individual::getPhenomCode(){
	return phenotype->returnPhenom();
}

//Return an individuals fitness value
int individual::getFitness(){
	return fitness;
}

//Mutate a genom by m percent
void individual::mutateGenom(float mutationPercentage){
	genotype->mutate(mutationPercentage);
}


void individual::incrementFitness(int increment){
	fitness += increment;
}