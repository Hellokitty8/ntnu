#ifndef GENOTYPES_H_
#define GENOTYPES_H_

#include <cmath>
#include <cstdlib> 
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class genotypes {
	private:
		int genomBitSize;
		vector<int> genom;
	public:
		genotypes(int genomSize);
		genotypes(vector<int> genomCode);
		vector<int> returnGenom();
		void mutate(float mutationPercentage);
};

#endif