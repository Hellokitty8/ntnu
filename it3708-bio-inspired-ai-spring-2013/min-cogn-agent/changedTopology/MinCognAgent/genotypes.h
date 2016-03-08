#ifndef GENOTYPES_H_
#define GENOTYPES_H_

#include <cmath>
#include <cstdlib> 
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

//Problem Specific
const int SEGMENT_SIZE = 8;
const int GENE_SEGMENTS = 49;
typedef bitset<SEGMENT_SIZE> Gene;

class genotypes {
	private:
		vector<Gene> genom;
	public:
		genotypes(int genomSize);
		genotypes(vector<Gene> genomCode);
		void mutate(float mutationPercentage);
		vector<Gene> returnGenom();
};

#endif