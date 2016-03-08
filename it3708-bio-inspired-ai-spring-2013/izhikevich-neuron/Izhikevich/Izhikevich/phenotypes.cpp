#include "phenotypes.h"
#include "helper.h"
helper pHelper;

phenotypes::phenotypes(){
	phenom.clear();
}

/*	*** Izhikevich Specific ***
	- For each gene
		- Convert to decimal representation
		- Normalize the value and fit it in the given parameters range
		- (Decimal Value / Max Representable Value (2^15-1)) * (Max interval value - Min interval Value) + Min Interval Value
		- Add to phenom
*/
phenotypes::phenotypes(vector<int> genomCode){

	int	accumulated = 0;
	float v;
	vector<int> segment;
	vector<int>::const_iterator first, last;

	for(int i = 0; i < 5; i++){
		first = genomCode.begin() + accumulated;
		last = genomCode.begin() + accumulated + SEGMENT_SIZE;
		segment.insert(segment.end(), first, last);
		v = pHelper.bin2dec(segment);
		v = ( v /(float)MAX_SEGMENT_VALUE ) * MAX_MINUS_MIN[i] + MIN_VALUES[i];
		phenom.push_back(v);
		segment.clear();
		accumulated += SEGMENT_SIZE;
	}
	
}

vector<float> phenotypes::returnPhenom(){
	return phenom;
}
