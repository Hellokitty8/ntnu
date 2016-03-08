#include "statistics.h"

statistics::statistics(){

}

float statistics::calculateGenerationStats(vector<individual*> generation, int g, ofstream& out){
	
	int size = generation.size();
	float avg, sum = 0, maximum = 0, fitness;

	for(int i = 0; i < size; i++){
		fitness = generation[i]->getFitness();
		if(fitness > maximum)
			maximum = fitness;
		sum += fitness;
	}

	avg = (float)sum/size;
	cout << "\nAverage Fitness: " << avg;
	sum = 0;

	for(int i = 0; i < size; i++){
		sum += pow(generation[i]->getFitness()-avg, 2);
	}

	out << g << " " << maximum << " " << avg << " " << sqrt(sum/size) << "\n";
	return maximum;
}

void statistics::writeParametersToFile(int populationSize, int generationLimit, float crossoverRate, int crossoverPoints,
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