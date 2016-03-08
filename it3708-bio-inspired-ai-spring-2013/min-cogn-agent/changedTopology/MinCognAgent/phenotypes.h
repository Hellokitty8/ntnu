#ifndef PHENOTYPES_H_
#define PHENOTYPES_H_

#include <iostream>
#include <vector>
#include <iostream>
#include "genotypes.h"
using namespace std;

//Problem Specific 
const int MAX_SEGMENT_VALUE = pow(2, SEGMENT_SIZE)-1;
const int NUM_WEIGHTS = 34;
const int BIAS_WEIGHTS = 5;
const int GAINS = 5;
const int TIME_CONSTANTS = 5;
const float MIN_VALUES[] = { -5.0f, -10.0f, 1.0f, 1.0f };
const float MAX_VALUES[] = { 5.0f, 0.0f, 5.0f, 2.0f };
const float RANGE[] = { 10.0f, 10.0f, 4.0f, 1.0f };

class phenotypes{
	private:
		vector<float> phenom;	
	public:
		phenotypes();
		phenotypes(vector<Gene> genomCode);
		vector<float> returnPhenom();
};

#endif