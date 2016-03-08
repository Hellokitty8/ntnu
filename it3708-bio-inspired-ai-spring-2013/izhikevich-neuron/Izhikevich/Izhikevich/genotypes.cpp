#include "genotypes.h"
#include "phenotypes.h"
#include "helper.h"

helper gHelper;

genotypes::genotypes(int genomSize) {
	for(int i = 0; i < genomSize; i++)
		genom.push_back(rand()%2);
}

genotypes::genotypes(vector<int> genomCode){
	genom = genomCode;
}

/*  *** Izhikevich Specific ****
	- Chooses a random gene to mutate
	- Mutates the gene by a given mutationPercentage
	- Makes sure that it is a legal value
	- Replaces old genom with the new
*/
void genotypes::mutate(float mutationPercentage){
	
	int variable = rand()%5,
		accu = variable*SEGMENT_SIZE,
		decimal,
		maxMutation,
		mutation,
		j = 0;
	vector<int> temp;
	temp.insert(temp.end(), genom.begin()+accu, genom.begin()+accu+SEGMENT_SIZE);
	decimal = gHelper.bin2dec(temp);
	maxMutation = ceil(MAX_SEGMENT_VALUE*mutationPercentage + 0.5f);
	mutation = (rand()%maxMutation)+1;
	decimal += (rand()%2) ? mutation : -mutation;
	decimal = min(decimal, MAX_SEGMENT_VALUE);
	decimal = max(decimal, 0);
	temp = gHelper.dec2bin(decimal, SEGMENT_SIZE);
	for (int i = accu; i < accu+SEGMENT_SIZE; i++){
		genom[i] = temp[j++];	
	}
	
}

//Return the genom
vector<int> genotypes::returnGenom(){
	return genom;
}

