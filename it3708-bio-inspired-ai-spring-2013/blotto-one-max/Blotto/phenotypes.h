#ifndef PHENOTYPES_H_
#define PHENOTYPES_H_

#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

class phenotypes{
	private:
		vector<float> phenom;	
	public:
		phenotypes();
		phenotypes(vector<int> genomCode);
		vector<float> returnPhenom();
};

#endif