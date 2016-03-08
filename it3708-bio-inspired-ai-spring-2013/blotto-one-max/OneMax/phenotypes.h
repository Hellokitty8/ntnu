#ifndef PHENOTYPES_H_
#define PHENOTYPES_H_

#include <iostream>
#include <vector>
#include <iostream>
using namespace std;

class phenotypes{
	private:
		vector<int> phenom;	
	public:
		phenotypes();
		phenotypes(vector<int> genomCode);
		vector<int> returnPhenom();
};

#endif