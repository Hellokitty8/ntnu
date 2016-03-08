#ifndef IZHIKEVICH_H_
#define IZHIKEVICH_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

const int MIN_SAMPING_DENSITY = 1;
const int MAX_SAMPLING_DENSITY = 50;

enum Metric { SPIKETIMEDISTANCE, SPIKEINTERVALDISTANCE, WAVEFORMDISTANCE };

class izhikevich {
	private:
		int timeSteps;
		int timeStepWindow;
		int spikeTreshold;
		int	effectiveTimeConstant;
		int	externalInput;
		int	outputValue;
		int	activationThreshold;
		int	spikeTrain;
		int	samplingDistance;
		Metric sdm;
		vector<vector<float> > izzyTrains;

	public:
		izhikevich();
		void readTrainingData();
		void setIzhievichVariables();
		float testPhenom(vector<float> parameters);
		float spikeTimeDistance(vector<float> t1, vector<float> t2);
		float spikeIntervalDistance(vector<float> t1, vector<float> t2);
		float waveformDistance(vector<float> t1, vector<float> t2, int T);
		float spikeCountDifferencePenalty(int n, int m, int l);
		vector<int> findSpikes(vector<float> spikeTrain, int T, int w);
		vector<float> createSpikeTrain(float a, float b, float c, float d, float k, int I, int t, int T);
		void writeToFile(vector<float>);

};
#endif