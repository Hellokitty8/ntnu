#include "phenotypes.h"
#include "helper.h"
helper pHelper;

phenotypes::phenotypes(){
	phenom.clear();
}

//From bitset to float vector
phenotypes::phenotypes(vector<int> genomCode){
	phenom = pHelper.bitVector2numVector(genomCode);
}


vector<float> phenotypes::returnPhenom(){
	return phenom;
}
