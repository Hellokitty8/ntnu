#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include <string>
#include <vector>
#include "genotypes.h"
#include "phenotypes.h"
using namespace std;

class individual {
	private:
		genotypes* genotype;
		phenotypes* phenotype;
		float fitness;

	public:
		individual(int genomSize);
		individual(vector<Gene> genomCode);
		~individual();
		void evolve();
		void setFitness(float fitnessScore);
		void mutateGenom(float mutationPercentage);
		vector<Gene> getGenomcode();
		float getFitness();
		vector<float> getPhenom();
};

#endif