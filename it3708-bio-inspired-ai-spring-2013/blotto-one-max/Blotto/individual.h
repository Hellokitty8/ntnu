#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include <string>
#include <vector>
#include <list>
class genotypes;
class phenotypes;

using namespace std;

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
		void mutateGenom(float mutationPercentage);
		void incrementFitness(int increment);
		vector<int> getGenomcode();
		vector<float> getPhenomCode();
		int getFitness();

};

#endif