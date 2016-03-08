#include "phenotypes.h"
#include "helper.h"
helper pHelper;

phenotypes::phenotypes(){
	phenom.clear();
}

//Decode genom and fit parameters in their specified range
phenotypes::phenotypes(vector<Gene> genomCode){

	int j = 0;
	float temp;
	
	for(int i = 0; i < NUM_WEIGHTS; i++){
		temp = (genomCode[i].to_ullong()/(float)MAX_SEGMENT_VALUE)*RANGE[0] + MIN_VALUES[0];
		phenom.push_back(temp);	
	}
	j = NUM_WEIGHTS;
	for(int i = j; i < j+BIAS_WEIGHTS; i++){
		temp = (genomCode[i].to_ullong()/(float)MAX_SEGMENT_VALUE)*RANGE[1] + MIN_VALUES[1];
		phenom.push_back(temp);
	}
	j += BIAS_WEIGHTS;
	for(int i = j; i < j+GAINS; i++){
		temp = (genomCode[i].to_ullong()/(float)MAX_SEGMENT_VALUE)*RANGE[2] + MIN_VALUES[2];
		phenom.push_back(temp);
	}
	j += GAINS;
	for(int i = j; i < j+TIME_CONSTANTS; i++){
		temp = (genomCode[i].to_ullong()/(float)MAX_SEGMENT_VALUE)*RANGE[3] + MIN_VALUES[3];
		phenom.push_back(temp);
	}	
}

vector<float> phenotypes::returnPhenom(){
	return phenom;
}
