#include "phenotypes.h"
#include "helper.h"
helper pHelper;


phenotypes::phenotypes(){
	phenom.clear();
}

//From bitset to integer list
phenotypes::phenotypes(vector<int> genomCode){
	phenom = genomCode;
}

vector<int> phenotypes::returnPhenom(){
	return phenom;
}
