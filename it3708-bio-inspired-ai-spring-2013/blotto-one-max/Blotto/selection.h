#ifndef SELECTION_H_
#define SELECTION_H_

#include <cmath>
#include <cstdlib> 
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include "individual.h"

using namespace std;

const int MAX_RANKSELECT_VAL = 2;
const int MIN_RANKSELECT_VAL = 0;
const int MAX_TOURNAMENT_SIZE = 10;
const int MIN_TOURNAMENT_SIZE = 2;

enum Mechanism { FITNESS, RANK, SIGMA, TOURNAMENT };
enum Protocol { FULLGENREP, OVERPROD, GENMIXING };

class selection {
	private:
		int selectionMechanism;
		int selectionProtocol;
		int overProductionLimit;
		int tournamentSize;
		float minRankValue;
		float maxRankValue;
		float e;
	public:
		selection();
		void setSelectionParameters(int populationSize);
		void adultAndParentSelection(vector<individual*> &juveniles, vector<individual*> &adults, vector<individual*> &parents, int populationSize, int elitesm);
		void fullGenReplacement(vector<individual*> &juveniles, vector<individual*> &adults);
		void overProduction(vector<individual*> &juveniles, vector<individual*> &adults);
		void generationMixing(vector<individual*> &juveniles, vector<individual*> &adults, int count, int elitism);
		void fitnessProportionate(vector<individual*> &adults, vector<individual*> &parents, int count);
		void sigmaScaling(vector<individual*> &adults, vector<individual*> &parents, int count);
		void rankSelection(vector<individual*> &adults, vector<individual*> &parents, int count);
		void tournamentSelection(vector<individual*> &adults, vector<individual*> &parents, int count, int elitism);
		vector<individual*> rouletteWheel(vector<float> probabilities, vector<individual*> selectFrom, int count);
		string returnMechanism();
		string returnProtocol();
		static bool fitnessSortFunc(individual* p1, individual* p2);
};

#endif