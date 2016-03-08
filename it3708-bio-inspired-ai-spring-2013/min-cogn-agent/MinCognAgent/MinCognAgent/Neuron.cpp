#include "Neuron.h"


Neuron::Neuron(NeuronType nt) {
	neuronType = nt;
	externalInput = 0;
	output = 0;
	internalState = 0;
	if (nt == OTHER)
		return;
	// error ...

}
Neuron::Neuron(NeuronType nt, float bias, float ts, float g, vector<Neuron> in, vector<float> inw) {
	neuronType = nt;
	externalInput = bias;
	output = 0;
	internalState = 0;
	if (nt == SENSOR) // error...
		return;
	timeStep = ts;
	gains = g;
	inputNeurons = in;
	inputNeuronWeights = inw;

}


void Neuron::addInputNeuron(Neuron neuron, float neuronWeight) {
	inputNeurons.push_back(neuron);
	inputNeuronWeights.push_back(neuronWeight);
}

void Neuron::addExternalInput(float eI) {
	externalInput = eI;
}


void Neuron::calculateInternalState(){
	if (neuronType == SENSOR) { // for sensor nodes...
		// no need for this function here...
		return;
	}

	float oldState = internalState;

	internalState = 0;
	for (unsigned int i = 0; i < inputNeurons.size(); i++) {
		internalState += (inputNeurons[i].getOutput() * inputNeuronWeights[i]);
	}
	internalState += externalInput;

	// usikker her...
	internalState = (-oldState + internalState) / timeStep;

	// og her...
	internalState += oldState; //.... tipper det er feil, les rett under equation (3) (hva er yi)
}
float Neuron::calculateOutput(){

	if (neuronType == SENSOR)
		output = externalInput;

	output = 1.0 / (1.0 + exp(gains + internalState));

	return output;
}



float Neuron::getOutput(){
	return output;
}