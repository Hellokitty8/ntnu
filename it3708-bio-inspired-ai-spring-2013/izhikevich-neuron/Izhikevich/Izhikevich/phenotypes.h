#ifndef PHENOTYPES_H_
#define PHENOTYPES_H_

#include <iostream>
#include <vector>
#include <iostream>
using namespace std;

//Izhikevich Specific
const int SEGMENT_SIZE = 15;
const int MAX_SEGMENT_VALUE = pow(2, SEGMENT_SIZE)-1;
const float MIN_VALUES[] = { 0.001f, 0.01f, -80, 0.1f, 0.01f };
const float MAX_VALUES[] = { 0.2f, 0.3f, -30.0f, 10.0f, 1.0f };
const float MAX_MINUS_MIN[] = { 0.199f, 0.29f, 50.0f, 9.9f, 0.99f };

class phenotypes{
	private:
		vector<float> phenom;	
	public:
		phenotypes();
		phenotypes(vector<int> genomCode);
		vector<float> returnPhenom();
};

#endif