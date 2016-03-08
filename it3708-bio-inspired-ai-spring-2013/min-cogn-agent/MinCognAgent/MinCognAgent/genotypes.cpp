#include "genotypes.h"
#include "phenotypes.h"
#include "helper.h"

helper gHelper;

genotypes::genotypes(int genomSize) {

	Gene n;
	
	for(int i = 0; i < genomSize; i++){
		for(int j = 0; j < SEGMENT_SIZE; j++){
			if(rand()%2){
				n.flip(j);
			}
		}
		genom.push_back(n);
		n.reset();
	}

}

genotypes::genotypes(vector<Gene> genomCode){
	genom = genomCode;
}

void genotypes::mutate(float mutationPercentage){
	
	//Problem?
	//Should mutate multiple genes in the genom
	/*
	int variable = rand()%GENE_SEGMENTS,
		decimal,
		maxMutation,
		mutation;

	decimal = genom[variable].to_ulong();	
	maxMutation = ceil(MAX_SEGMENT_VALUE*mutationPercentage + 0.5f);
	mutation = (rand()%maxMutation)+1;
	decimal += (rand()%2) ? mutation : -mutation;
	decimal = min(decimal, MAX_SEGMENT_VALUE);
	decimal = max(decimal, 0);
	Gene n(decimal); 
	genom[variable] = n;
	*/
	
	//Alternative Mutation Function
	//Mutates multiple genes instead of just one

	Gene* newGene;
	int gene, mutationCount, decimal, maxMutation, mutation;
	mutationCount = floor(mutationPercentage*GENE_SEGMENTS*5 + 0.5f);

	for(int i = 0; i < mutationCount; i++){

		gene = rand()%GENE_SEGMENTS;
		decimal = genom[gene].to_ulong();	
		maxMutation = ceil(MAX_SEGMENT_VALUE*mutationPercentage + 0.5f);
		mutation = (rand()%maxMutation)+1;
		decimal += (rand()%2) ? mutation : -mutation;
		decimal = min(decimal, MAX_SEGMENT_VALUE);
		decimal = max(decimal, 0);
		newGene = new Gene(decimal);
		genom[gene] = *newGene;

	}
	
	
}

//Return the genom
vector<Gene> genotypes::returnGenom(){
	return genom;
}

