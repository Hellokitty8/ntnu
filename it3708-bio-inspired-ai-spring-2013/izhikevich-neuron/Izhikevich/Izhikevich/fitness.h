#ifndef FITNESS_H_
#define FITNESS_H_

#include <vector>
using namespace std;

class individual; 

class fitness{
	private:
		float bestFitness;
		vector<float> bestPhenom;
	public:
		fitness();
		void setFitnessVariables(int genomSize);
		bool testFitness(vector<individual*> generation);
		void writeBestSolutionToFile();
		void reset();
};

#endif