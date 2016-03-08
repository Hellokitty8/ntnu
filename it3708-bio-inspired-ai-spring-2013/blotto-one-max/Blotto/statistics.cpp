#include "statistics.h"
#include "helper.h"
helper statHelper;

statistics::statistics(){

}

vector<float> statistics::calculateGenerationStats(vector<individual*> generation){
	
	vector<float> result, prios;
	int size = generation.size(),
		sum = 0,
		maximum = 0,
		fitness = 0;
	float avg, runningEntropyAvg = 0, tempSum;

	for(int i = 0; i < size; i++){
		fitness = generation[i]->getFitness();
		if(fitness > maximum){
			maximum = fitness;
		}
		sum += fitness;
	}

	avg = static_cast<float>(sum) / static_cast<float>(size);
	sum = 0;
	for(int i = 0; i < size; i++){
		sum += pow(generation[i]->getFitness()-avg, 2);
	}

	// Blotto - Entrophy
	for (int i = 0; i < size; i++) {
		prios = statHelper.bitVector2numVector(generation[i]->getGenomcode());
		tempSum = 0;
		for (int j = 0; j < prios.size(); j++)  {
			if (prios[j] == 0) { // hack
				continue;
			}
			tempSum += ( prios[j] * (log(prios[j])/log(2)) );
		}
		runningEntropyAvg -= tempSum;
	}
	
	result.push_back(maximum);
	result.push_back(avg);
	result.push_back(sqrt(sum/size));
	result.push_back(runningEntropyAvg / size);

	return result;
}

void statistics::writeDataToFile(int populationSize, int generationLimit, float crossoverRate, int crossoverPoints,
								 float mutationPercentage, float bitWiseMutation, int elitism,
								 int generationCount, string protocol, string mechanism){
	
	fstream statistics;
	statistics.open ("statistics.txt", fstream::in | fstream::out | fstream::app);

	statistics << populationSize << " " << crossoverRate << " " << crossoverPoints << " " << mutationPercentage << " ";
	statistics << bitWiseMutation << " " << elitism << " " << protocol << " " << mechanism << " ";
	if(generationCount == generationLimit-1)
		statistics << "Couldnt Find a Solution" << "\n";
	else
		statistics << generationCount << "\n";

	statistics.close();
}