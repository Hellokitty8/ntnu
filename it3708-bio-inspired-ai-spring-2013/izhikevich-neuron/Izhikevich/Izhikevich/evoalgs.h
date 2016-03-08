#ifndef EVOALGS_H_
#define EVOALGS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "individual.h"
#include "fitness.h"
#include "selection.h"
#include "statistics.h"

using namespace std;

const int MIN_GENERATIONS = 20;
const int MAX_GENERATIONS = 5000;
const int MIN_CROSSOVER = 1;
const int MAX_CROSSOVER = 10;
const float MIN_CROSSOVER_RATE = 0.1f;
const float MAX_CROSSOVER_RATE = 1.0f;
const float MIN_MUTATION = 0.01f;
const float MAX_MUTATION = 1.0f;
const float MIN_BITWISE_MUTATION = 0.01f;
const float MAX_BITWISE_MUTATION = 0.6f;
const int MIN_ELITISM = 0;
const int MAX_ELITISM = 4;
const int MIN_POPULATION = 5;
const int MAX_POPULATION = 1000;
const int MIN_GENOM = 5;
const int MAX_GENOM = 100;
const int MIN_TRESHOLD = 0;
const int MAX_TRESHOLD = 100;
const int SEGMENT_CROSSOVER_POINTS = 4;
const int SEGMENT_SIZE = 15;
const int STAGNANT_LIMIT = 10;

class evoalgs {
	private:
		int genomSize;
		int generationCount;
		int lastProgress;
		int populationSize;
		int generations;
		int crossoverPoints;
		int elitism;
		int treshold;
		float crossoverRate;
		float mutationPercentage;
		float mutation;
		vector<individual*> youths;
		vector<individual*> adults;
		vector<individual*> parents;
		
	public:
		evoalgs();
		void setEAParameters();
		void evolutionCycle();
		void reset();
		void generateChildren();
		void evolveChildren();
		void reproduction();
		void randomCrossover(individual* a, individual* b, int numberOfChildren);
		void crossover(individual* a, individual* b, int numberOfChildren);
		void deletePopulation();
};

#endif