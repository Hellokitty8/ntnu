#ifndef FITNESS_H_
#define FITNESS_H_

class individual; 
#include <vector>

using namespace std;

const float MAX_REPLOYMENT = 1.0f;
const float MIN_REPLOYMENT = 0.0f;
const float MIN_LOSSFRACTION = 0.0f;
const float MAX_LOSSFRACTION = 1.0f;

class fitness{
	private:
		int battleN;
		float reploymentFactor;
		float lossFraction;
	public:
		fitness();
		void setFitnessVariables(int);
		void testFitness(vector<individual*> generation);
		void troopRedistribution(vector<float>*, int, float);
		void strengthReduction(vector<float>*, int, float);	
};

#endif