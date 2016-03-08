#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include "individual.h"
using namespace std;


class statistics {
	private:
	public:
		statistics();
		vector<float> calculateGenerationStats(vector<individual*> generation);
		void writeDataToFile(int populationSize, int generationLimit, float crossoverRate, int crossoverPoints,
							float mutationPercentage, float bitWiseMutation, int elitism,
							int generationCount, string protocol, string mechanism);
};

#endif