#include "statistics.h"

statistics::statistics(){

}

vector<float> statistics::calculateGenerationStats(vector<individual*> generation){
	
	int size = generation.size();
	float avg, sum = 0, maximum = 0, fitness;
	vector<float> result;

	for(int i = 0; i < size; i++){
		fitness = generation[i]->getFitness();
		if(fitness > maximum)
			maximum = fitness;
		sum += fitness;
	}
	avg = (float)sum/size;
	sum = 0;
	for(int i = 0; i < size; i++)
		sum += pow(generation[i]->getFitness()-avg, 2);
	result.push_back(maximum);
	result.push_back(avg);
	result.push_back(sqrt(sum/size));

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