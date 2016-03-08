#include "helper.h"

//Converts a vector of 4 bit integers to a decimal vector, then normalizes the values
vector<float> helper::bitVector2numVector(vector<int> bits)
{
	vector<float> numList;
	int sum = 0;

	for (int i = 0; i < bits.size(); i = i + 10)
	{
		float decimal = 0;
		for (int j = 0; j < 10; j++)
			decimal += bits[i+j];
		sum = sum + decimal;
		numList.push_back(decimal);
	}

	for (int i = 0; i < numList.size(); i++)
		numList[i] = numList[i] / sum;
	return numList;
}

//Function for easing reading of new int variables
int helper::readInt(string str, int min, int max){
	
	int v;
	do{
		cout << str << " (" << min << "-" << max << "): ";
		cin >> v;
	}while(v < min || v > max);
	return v;
}

//Function for easing reading of new flaot variables
float helper::readFloat(string str, float min, float max){
	
	float v;
	do{
		cout << str << " (" << min << "-" << max << "): ";
		cin >> v;
	}while(v < min || v > max);
	return v;
}

void helper::printPhenom(individual* individ, ofstream& str) {
	for (int i = 0; i < individ->getPhenomCode().size(); i++)
	{
		str << ("%.2f", individ->getPhenomCode()[i])  << " ";
	}
	str << endl;
}