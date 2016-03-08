#include "izhikevich.h"
#include "helper.h"
helper iHelper;

izhikevich::izhikevich(){
	timeSteps = 1000;
	timeStepWindow = 5;
	spikeTreshold = 35;
	effectiveTimeConstant = 10;
	externalInput = 10;
	outputValue = 10;
	activationThreshold = 0;
	readTrainingData();
}

void izhikevich::readTrainingData(){

	float t;
	vector<float> temp;
	string filePaths[] = {"training_data/izzy-train1.dat", "training_data/izzy-train2.dat", "training_data/izzy-train3.dat", "training_data/izzy-train4.dat"};
	ifstream trainingData;
		
	for(int i = 0; i < 4; i++){
		trainingData.open(filePaths[i]);
		if(trainingData.is_open()){
			do{
				trainingData >> t;
				temp.push_back(t);	
			}while(!trainingData.eof());
		}
		izzyTrains.push_back(temp);
		temp.clear();
		trainingData.close();
	}
}

void izhikevich::setIzhievichVariables(){

	int command;
	spikeTrain = iHelper.readInt("Compare to Spike Train", 1, 4) - 1;
	command = iHelper.readInt("SDM\n1: Spike Time Distance Metric\n2: Spike Interval Distance Metric\n3: Waveform Distance Metric\n", 1, 3);
	switch(command){
		case 1: sdm = SPIKETIMEDISTANCE; break;
		case 2: sdm = SPIKEINTERVALDISTANCE; break;
		case 3: sdm = WAVEFORMDISTANCE; break;
	}
	samplingDistance = (sdm==WAVEFORMDISTANCE) ? iHelper.readInt("Samping Density", MIN_SAMPING_DENSITY, MAX_SAMPLING_DENSITY) : 0;
}

/*   *** Fitness testing of phenom ***
	- Create a spike train by using the phenoms parameters
	- Depending on the selected similarity metric
		- Compare the spike train to the selected target
	- Return the 1 / the calculated similarity
*/
float izhikevich::testPhenom(vector<float> p){
	
	float result;
	vector<float> phenom = createSpikeTrain(p[0], p[1], p[2], p[3], p[4], externalInput, effectiveTimeConstant, spikeTreshold);
	switch(sdm){
		case SPIKETIMEDISTANCE:	result = spikeTimeDistance(phenom, izzyTrains[spikeTrain]);	break;
		case SPIKEINTERVALDISTANCE:	result = spikeIntervalDistance(phenom, izzyTrains[spikeTrain]);	break;
		case WAVEFORMDISTANCE:	result = waveformDistance(phenom, izzyTrains[spikeTrain], samplingDistance); break;
	}
	return 1/(1+result);
}


//Izhikevich-Model
vector<float> izhikevich::createSpikeTrain(float a, float b, float c, float d, float k, int I, int t, int T){

	float u, v, dU, dV;
	vector<float> result;

	u = 0;
	v = -60;

	result.push_back(v);
	
	for(int i = 0; i <= timeSteps; i++){

		dV = ((k*pow(v, 2)) + 5*v + 140 - u + I )/(float)t;
		dU = (a/(float)t)*( b*v - u );
		result.push_back(v);
		if(v >= T){
			v = c;
			u += d;
		}
		else{
			v += dV;
			u += dU;
		}	
	}
	return result;
}

/*    *** Find the spike positions ***
	- When the spiketrain value exceeds the treshold
	- Find the local maximum within the next 5 ms
	- And save the spike position
*/
vector<int> izhikevich::findSpikes(vector<float> spikeTrain, int T=0, int w=5){
	
	int index, last, size = spikeTrain.size();
	float max;
	vector<int> result;

	for(int i = 0; i < size; i++){

		if(spikeTrain[i] >= T){
			max = spikeTrain[i];
			index = i;		
			last = (i+w > size) ? size : (i+w);
			for(int j = i+1; j < last; j++){		
				if(spikeTrain[j] > max){
					max = spikeTrain[j];
					index = j;
				}
			}
			result.push_back(index);
			i += w;
		}
	}
	return result;
}

/*	*** Spike Time Distance Metric ***
	- Compare the positions of the spikes of both trains
*/
float izhikevich::spikeTimeDistance(vector<float> t1, vector<float> t2){
	
	int min;
	float result = 0;
	vector<int> s1 = findSpikes(t1);
	vector<int> s2 = findSpikes(t2);
	min = ( s1.size() > s2.size() ) ? s2.size() : s1.size();	
		
	for(int i = 0; i < min; i++){
		result += pow(abs(s1[i] - s2[i]), 2);
	}
	result = sqrt(result) + spikeCountDifferencePenalty(s1.size(), s2.size(), t1.size());
	result = result / (float)(min-1);
	return result;
	
}

/*	*** Spike Interval Distance Metric ***
	- Compare the distances between the spikes
	- Ignoring the time of the occurence of the spike
*/
float izhikevich::spikeIntervalDistance(vector<float> t1, vector<float> t2){
	
	int min;
	float result = 0;
	vector<int> s1 = findSpikes(t1);
	vector<int> s2 = findSpikes(t2);

	min = ( s1.size() > s2.size() ) ? s2.size() : s1.size();	
		
	for(int i = 1; i < min; i++){
		result += pow(abs((s1[i]-s1[i-1]) - (s2[i]-s2[i-1])), 2);
	}

	result = sqrt(result) + spikeCountDifferencePenalty(s1.size(), s2.size(), t1.size());
	result = result / (float)(min-1);
	return result;
	
}

/*	*** Waveform Distance Metric ***
	- Compare the distance between 2 spike trains
	- By comparing the distance of the values at each Tth step
*/
float izhikevich::waveformDistance(vector<float> t1, vector<float> t2, int T){

	int samples = 0;
	float result = 0;

	for(int i = 0; i < t1.size(); i+=T){
		result += pow(abs(t1[i] - t2[i]), 2);
		samples++;
	}
	result = sqrt(result);
	return result/(float)samples;

}

float izhikevich::spikeCountDifferencePenalty(int n, int m, int l){
	m = max(m, 1);
	return (abs(n-m)*l)/(float)(2*m);
}

//Write the best found spiketrain and variables to file
void izhikevich::writeToFile(vector<float> p) {

	ofstream izFile;
	izFile.open("spiketrains.txt");
	vector<float> fittest = createSpikeTrain(p[0], p[1], p[2], p[3], p[4], externalInput, effectiveTimeConstant, spikeTreshold);

	for (int i = 0; i < izzyTrains[spikeTrain].size(); i++){
		izFile << izzyTrains[spikeTrain].at(i) << " ";
	}
	
	izFile <<  "\n";

	for (int i = 0; i < fittest.size(); i++){
		izFile << fittest.at(i) << " ";
	}

	izFile << "\n";

	for(int i = 0; i < p.size(); i++){
		izFile << p[i] << " ";
	}

	izFile.close();

}


