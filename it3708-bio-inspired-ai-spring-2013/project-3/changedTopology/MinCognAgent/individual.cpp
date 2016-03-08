#include "individual.h"
#include "genotypes.h"
#include "phenotypes.h"

individual::individual(int genomSize){
	genotype = new genotypes(genomSize);
	phenotype = new phenotypes();	
}

individual::individual(vector<Gene> genomCode){
	genotype = new genotypes(genomCode);
	phenotype = new phenotypes();
}

individual::~individual(){
	delete genotype;
	delete phenotype;
}

vector<Gene> individual::getGenomcode(){
	return genotype->returnGenom();
}

float individual::getFitness(){
	return fitness;
}

void individual::mutateGenom(float mutationPercentage){
	genotype->mutate(mutationPercentage);
}

void individual::setFitness(float fitnessScore){
	fitness = fitnessScore;
}

void individual::evolve(){
	delete phenotype;
	phenotype = new phenotypes(genotype->returnGenom());
}

vector<float> individual::getPhenom() {
	return phenotype->returnPhenom();
}