#ifndef FITNESS_H_
#define FITNESS_H_

#include <vector>
class individual; 
using namespace std;

class fitness{
	private:
		vector<int> solutionGenom;
	public:
		fitness();
		void setFitnessVariables(int genomSize);
		void testFitness(vector<individual*> generation);	
};

#endif