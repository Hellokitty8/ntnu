#include "genotypes.h"

genotypes::genotypes(int genomSize) {
	for(int i = 0; i < genomSize; i++)
		genom.push_back(rand()%2);
}

genotypes::genotypes(vector<int> genomCode){
	genom = genomCode;
}

//Mutation function, flips percentage of the bits
void genotypes::mutate(float mutationPercentage){
	
	int r,
		genomSize = genom.size();
	float bitsToMutate = (float)genomSize*mutationPercentage;
	bitsToMutate = floor(bitsToMutate + 0.5f);
	
	for(int i = 0; i < bitsToMutate; i++){
		r = rand()%genomSize;
		genom[r] = (genom[r] == 1) ?  0 : 1;
	}
}

//Return the genom
vector<int> genotypes::returnGenom(){
	return genom;
}

