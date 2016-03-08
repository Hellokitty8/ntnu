#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class genotypes;
class phenotypes;

class individual {
	private:
		genotypes* genotype;
		phenotypes* phenotype;
		float fitness;

	public:
		individual(int genomSize);
		individual(vector<int> genomCode);
		~individual();
		void evolve();
		void setFitness(float fitnessScore);
		void mutateGenom(float mutationPercentage);
		vector<int> getGenomcode();
		float getFitness();
		vector<float> getPhenom();
};

#endif