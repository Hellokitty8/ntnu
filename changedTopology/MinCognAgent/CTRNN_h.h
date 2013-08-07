#ifndef CTRNN_H_H_
#define CTRNN_H_H_

#include <vector>
#include "Neuron.h"



const int NOF_SENSOR_NEURONS = 5;
const int NOF_HIDDEN_NEURONS = 2;
const int NOF_MOTOR_NEURONS = 2;

using namespace std;





class CTRNN_h {
	private:
		vector<Neuron> sensorNeurons;
		vector<Neuron> hiddenNeurons;
		vector<Neuron> motorNeurons;
	public:
		CTRNN_h(vector<float> gains, vector<float> bias, vector<float> timesteps, vector<float> outWeights);
		vector<float> runTimeStep(vector<float>);
		
		
		
};


#endif