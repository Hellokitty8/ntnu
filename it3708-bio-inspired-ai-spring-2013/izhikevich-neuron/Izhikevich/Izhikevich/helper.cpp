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

int helper::bin2dec(vector<int> binary){

	int dec = 0, size = binary.size();
	for(int i = 0; i < size; i++){
		if(binary[size-i-1])
			dec += pow(2, i);
	}
	return dec;
}

vector<int> helper::dec2bin(int dec, int bits) {
	vector<int> binary;
	binary.assign(bits, 0);
	while(dec >= 1){
		binary[--bits] = dec%2;
		dec /= 2;
	}
	return binary;
}
