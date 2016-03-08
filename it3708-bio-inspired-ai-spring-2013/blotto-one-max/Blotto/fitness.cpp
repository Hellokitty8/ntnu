#include "fitness.h"
#include "individual.h"
#include "helper.h"
#include <algorithm>
helper fHelper;

fitness::fitness(){
	
}

void fitness::troopRedistribution(vector<float>* dist, int index, float difference){
	
	float n = dist->size() - index + 1;
	for (int i = index + 1; i < dist->size(); i++) {
		dist->at(i) = dist->at(i) + ((difference/n) * reploymentFactor);
	}
}

void fitness::testFitness(vector<individual*> generation){
	
	int iWin, jWin;
	vector<float> iGen, jGen;
	float iStrengthFactor, iArmy, jStrengthFactor, jArmy;
	
	for (int i = 0; i < (generation.size() - 1); i++) {
		for (int j = (i + 1); j < generation.size(); j++){
			
			jGen = generation[j]->getPhenomCode();
			iGen = generation[i]->getPhenomCode();
			iWin = 0;
			jWin = 0;
			iStrengthFactor = 1.0; 
			jStrengthFactor = 1.0;

			//Let the armies fight battle for battle
			for (int b = 0; b < battleN; b++) {

				iArmy = iGen[b] * iStrengthFactor;
				jArmy = jGen[b] * jStrengthFactor;
				if (iArmy > jArmy){
					iWin++;
					troopRedistribution(&iGen, b, iArmy - jArmy);
					jStrengthFactor = (jStrengthFactor - lossFraction > 0) ? (jStrengthFactor - lossFraction) : 0;
				}
				else if (iArmy < jArmy){
					jWin++;
					troopRedistribution(&jGen, b, jArmy - iArmy);
					iStrengthFactor = (iStrengthFactor - lossFraction > 0) ? (iStrengthFactor - lossFraction) : 0;
				}
			}
			//Update fitness values based on outcome
			if(iWin > jWin) 
				generation[i]->incrementFitness(2);
			else if (iWin < jWin) 
				generation[j]->incrementFitness(2);
			else{ 
				generation[i]->incrementFitness(1);
				generation[j]->incrementFitness(1);
			}
		}	
	}
}

void fitness::setFitnessVariables(int battleNum){

	battleN = battleNum;
	lossFraction = fHelper.readFloat("Loss Fraction", MIN_LOSSFRACTION, MAX_LOSSFRACTION);
	reploymentFactor = fHelper.readFloat("Troop Reployment Factor", MIN_REPLOYMENT, MAX_REPLOYMENT);

}