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
		int fitness;

	public:
		individual(int genomSize);
		individual(vector<int> genomCode);
		~individual();
		void evolve();
		void setFitness(int fitnessScore);
		void mutateGenom(float mutationPercentage);
		vector<int> getGenomcode();
		int getFitness();
};

#endif