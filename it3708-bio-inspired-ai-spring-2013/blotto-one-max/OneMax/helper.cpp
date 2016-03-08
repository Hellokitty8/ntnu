#include "helper.h"


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

