#include "genotypes.h"
#include "helper.h"

genotypes::genotypes(int genomSize) {

	genomBitSize = genomSize * 10;
	for(int i = 0; i < genomBitSize; i++){
		genom.push_back(rand()%2);
	}
}

genotypes::genotypes(vector<int> genomCode){

	genomBitSize = genomCode.size();
	genom = genomCode;
}

//Return the genom
vector<int> genotypes::returnGenom(){

	return genom;
}

//Mutation function, flips percentage of the bits
void genotypes::mutate(float mutationPercentage){
	
	int r;
	float bitsToMutate = (genom.size())*mutationPercentage;
	bitsToMutate = floor(bitsToMutate + 0.5f);

	for(int i = 0; i < bitsToMutate; i++){
		r = rand()%genomBitSize;
		genom[r] = ((genom[r] == 1) ?  0 : 1);
	}
}

