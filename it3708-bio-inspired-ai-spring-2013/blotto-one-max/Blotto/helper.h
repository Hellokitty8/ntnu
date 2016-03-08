#ifndef HELPER_H_
#define HELPER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include "individual.h"

using namespace std;

class helper{
	public:
		vector<float> bitVector2numVector(vector<int> bitVector);
		int readInt(string str, int min, int max);
		float readFloat(string str, float min, float max);
		void printPhenom(individual* individ, ofstream& os);
};



#endif
