#include "CTRNN_h.h"

// constructor sets all constant variables(bias, gains, timesteps, weights) and creates all neuron objects.
CTRNN_h::CTRNN_h(vector<float> gains, vector<float> bias, vector<float> timesteps, vector<float> weights){
	// add sensor neurons with externalInput values.
	for (int i = 0; i < NOF_SENSOR_NEURONS; i++){
		sensorNeurons.push_back(Neuron(SENSOR));
	}

	// add hidden neurons and links with weights from sensor neurons to hidden neurons.
	int weightsIndex = 0;
	int timestepsIndex = 0;
	int biasIndex = 0;
	int gainsIndex = 0;

	vector<float>::iterator start = weights.begin();
	vector<float>::iterator end = weights.begin() + NOF_SENSOR_NEURONS;
	vector<float> tempVector;

	for (int i = 0; i < NOF_SENSOR_NEURONS; i++){
		tempVector.insert(tempVector.begin(), start, end);
		
		hiddenNeurons[i] = Neuron(OTHER, bias[biasIndex++], timesteps[timestepsIndex++], gains[gainsIndex++], sensorNeurons, tempVector);
		
		tempVector.clear();
		start = end;
		end += NOF_SENSOR_NEURONS;
		weightsIndex += NOF_SENSOR_NEURONS;
	}

	// add links + weights between hidden neurons.
	for (int i = 0; i < NOF_HIDDEN_NEURONS; i++) {
		// self link!
		hiddenNeurons[i].addInputNeuron(hiddenNeurons[i], weights[weightsIndex++]);
		for (int j = i + 1; j < NOF_HIDDEN_NEURONS; j++) {
			hiddenNeurons[i].addInputNeuron(hiddenNeurons[j], weights[weightsIndex++]);
			hiddenNeurons[j].addInputNeuron(hiddenNeurons[i], weights[weightsIndex++]);
		}
	}

	// add motor neurons with links w/weights from hidden neurons.
	start = weights.begin() + weightsIndex;
	end = start + NOF_HIDDEN_NEURONS;

	for (int i = 0; i < NOF_MOTOR_NEURONS; i++) {
		tempVector.insert(tempVector.begin(), start, end);
		
		motorNeurons[i] = Neuron(OTHER, bias[biasIndex++], timesteps[timestepsIndex++], gains[gainsIndex++], hiddenNeurons, tempVector);

		tempVector.clear();
		start = end;
		end +=NOF_HIDDEN_NEURONS;
		weightsIndex += NOF_HIDDEN_NEURONS;

	}

	// add links + weights between hidden neurons.
	for (int i = 0; i < NOF_MOTOR_NEURONS; i++) {
		// self link
		motorNeurons[i].addInputNeuron(motorNeurons[i], weights[weightsIndex++]);
		for (int j = i + 1; j < NOF_MOTOR_NEURONS; j++) {
			motorNeurons[i].addInputNeuron(motorNeurons[j], weights[weightsIndex++]);
			motorNeurons[j].addInputNeuron(motorNeurons[i], weights[weightsIndex++]);
		}
	}
}



vector<float> CTRNN_h::runTimeStep(vector<float> externalInput) {
	// set external Input for sensor neurons.
	for (unsigned int i = 0; i < sensorNeurons.size(); i++) {
		sensorNeurons[i].addExternalInput(externalInput[i]);
		sensorNeurons[i].calculateOutput();
	}

	// I understand equation (1) to mean outputs from the previous timestep (equation 2 & 3). Else it makes no sense to have self links or links between nodes of similar level.
	// else self link is an infinite loop :D!
	// TODO: think harder.

	// alternative: calculate hidden states -> calculate hidden out -> calculate motor states -> calculate motor output

	// calculate internal states.
	for (unsigned int i = 0; i < hiddenNeurons.size(); i++)
		hiddenNeurons[i].calculateInternalState();
	for (unsigned int i = 0; i < motorNeurons.size(); i++)
		motorNeurons[i].calculateInternalState();
	// calculate outputs.
	for (unsigned int i = 0; i < hiddenNeurons.size(); i++)
		hiddenNeurons[i].calculateOutput();;
	
	vector<float> resultSet;
	for (unsigned int i = 0; i < motorNeurons.size(); i++)
		resultSet.push_back(motorNeurons[i].calculateOutput());
	
	return resultSet;

}