#ifndef NEURON_H
#define NEURON_H

#include <array>
#include <vector>
#include <math.h>


enum NeuronType{
	SENSOR,
	OTHER,
};

using namespace std;
class Neuron {
private:
	NeuronType neuronType;
	vector<Neuron> inputNeurons;
	vector<float> inputNeuronWeights;
	
	float externalInput;
	float gains;
	float timeStep;
	float internalState;
	float output;	
public:
	Neuron(NeuronType nt);
	Neuron(NeuronType nt, float externalInput, float timeStep, float gains, vector<Neuron> inputNeurons, vector<float> inputNeuronWeights);
	void addInputNeuron(Neuron n, float nw);
	void addExternalInput(float eI);
	void calculateInternalState();
	float calculateOutput();

	float getOutput();
};

#endif