#include "fitness.h"
#include "individual.h"
#include "izhikevich.h"

izhikevich izh;

fitness::fitness(){
	bestFitness = 0;
}

//Fitness testing function
bool fitness::testFitness(vector<individual*> generation)
{
	float fit;
	bool newBest = false;

	for (int i = 0; i < generation.size(); i++) {
		fit = izh.testPhenom(generation[i]->getPhenom());
		if (fit > bestFitness)
		{
			bestFitness = fit;
			bestPhenom = generation[i]->getPhenom();
			newBest = true;
		}
		generation[i]->setFitness(fit);
	}
	return newBest;
}

//Set the variables used in the fitness class
void fitness::setFitnessVariables(int genomSize){
	izh.setIzhievichVariables();
}

void fitness::writeBestSolutionToFile() {
	izh.writeToFile(bestPhenom);
}

void fitness::reset(){
	bestFitness = 0;
}
