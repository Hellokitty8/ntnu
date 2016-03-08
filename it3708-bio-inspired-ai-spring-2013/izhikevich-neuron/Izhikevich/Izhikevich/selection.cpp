#include "selection.h"
#include "helper.h"
helper sHelper;


selection::selection(){

}

//Function for setting the selection parameters
void selection::setSelectionParameters(int populationSize){

	int choice, choice2;
	float temp;

	cout << "\nSelection Mechanism\n1: Fitness-Proportionate\n2: Rank\n3: Sigma-Scaling\n4: Tournament Selection\n";
	do{
	cin >> choice;
	switch(choice){
		case 1: selectionMechanism = FITNESS; break;
		case 2: selectionMechanism = RANK;
				do{
					cout << "\nSelect min and max value for rank selection\n(Typical setting Min = 0.5, Max = 1.5 \nor Min = 0.0, Max = 2.0 for maximum selection pressure)";
					cout << "\nMin: ";
					cin >> minRankValue;
					cout << "Max: ";
					cin >> maxRankValue;
				}while(minRankValue > maxRankValue  || maxRankValue > MAX_RANKSELECT_VAL || minRankValue < MIN_RANKSELECT_VAL);
		case 3: selectionMechanism = SIGMA; break;
		case 4: selectionMechanism = TOURNAMENT;
				tournamentSize = sHelper.readInt("Tournament Size", MIN_TOURNAMENT_SIZE, MAX_TOURNAMENT_SIZE);
				e = sHelper.readFloat("Probability that the fittest individual wins", 0.1, 1.0);
				break;
		default: cout << "Invalid number!\n"; break;
	}
	}while(choice < 1 || choice > 4);

	cout << "\nSelection Protocol\n1: Full Generational Replacement \n2: Over-Production\n3: Generational Mixing\n";

	do{
		cin >> choice;
		switch(choice){
			case 1: selectionProtocol = FULLGENREP; break;
			case 2: selectionProtocol = OVERPROD;
					do{
						cout << "\nSelection Protocol chosen was 'Over production'.\nSelect adult pool size as a fraction of child pool size. (population size)\n1: 1/4 \n2: 1/2 \n3: 3/4\n";
						cin >> choice2;
						switch(choice2) {
							case 1:	temp = (float)populationSize/4; break;
							case 2: temp = (float)populationSize/2; break;
							case 3: temp = (float)(3*populationSize)/4; break;
							default: cout << "Invalid number!\n"; break;
						}
					}while(choice2 < 1 || choice2 > 3);
					overProductionLimit = floor(temp + 0.5f);
					break;
			case 3: selectionProtocol = GENMIXING; break;
			default: cout << "Invalid number!\n"; break;
		}
	} while(choice < 1 || choice > 3);

}


void selection::adultAndParentSelection(vector<individual*> &juveniles, vector<individual*> &adults, vector<individual*> &parents, int populationSize, int elitism){

	parents.clear();
	int count = populationSize - elitism;

	sort(juveniles.begin(), juveniles.end(), fitnessSortFunc);

	for(int i = 0; i < elitism; i++){
		parents.push_back(juveniles[i]);
	}
	
	if(selectionProtocol == FULLGENREP){             //Use the entire new generation
		fullGenReplacement(juveniles, adults);
	}
	else if(selectionProtocol == OVERPROD){          //Limit number of adults spots for the new generation     
		overProduction(juveniles, adults);
	}
	else { //protocol = GENMIXING){					//Survival of the fittest (both generations)
		generationMixing(juveniles, adults, count, elitism);
	}

	juveniles.clear();

	if(selectionMechanism == FITNESS){             //Fitness proportionate
		fitnessProportionate(adults, parents, count);
	}
	else if(selectionMechanism == SIGMA){          //Sigma Scaling
		sigmaScaling(adults, parents, count);
	}
	else if(selectionMechanism == RANK){           //Rank Selection
		rankSelection(adults, parents, count);
	}
	else {	//selectionMechanism == TOURNAMENT     //Tournament selection
		tournamentSelection(adults, parents, count, elitism);
	}

}

void selection::fullGenReplacement(vector<individual*> &juveniles, vector<individual*> &adults){
	
	vector<individual*>::iterator it;

	for(it = adults.begin(); it < adults.end();){
			delete * it;					//Call destructor on removed objects
			it = adults.erase(it);			//Erase from vector
	}
	adults = juveniles;

}

void selection::overProduction(vector<individual*> &juveniles, vector<individual*> &adults){
	
	vector<individual*>::iterator it;

	for(it = adults.begin(); it < adults.end();){
		delete * it;					//Call destructor on removed objects
		it = adults.erase(it);			//Erase from vector
	}
	adults.clear();
	sort(juveniles.begin(), juveniles.end(), fitnessSortFunc);	   //Sort phenoms based on fitness, descending order
	for(it = juveniles.begin()+overProductionLimit; it != juveniles.end();){ //Remove the unfit ones
		delete * it;											   //Destructor for object
		it = juveniles.erase(it);								   //Erase from vector
	}
	adults = juveniles;	

}

void selection::generationMixing(vector<individual*> &juveniles, vector<individual*> &adults, int count, int elitism){
	
	vector<individual*>::iterator it;

	juveniles.insert(juveniles.end(), adults.begin(), adults.end());	//Add juviniles to adult vector
	sort(juveniles.begin(), juveniles.end(), fitnessSortFunc);				//Sort after fitness value
	for(it = juveniles.begin()+count+elitism; it != juveniles.end();){		//Remove the unfit ones
		delete * it;													//Call Destructor
		it = juveniles.erase(it);											//Erase from vector
	}
	adults = juveniles;

}

void selection::fitnessProportionate(vector<individual*> &adults, vector<individual*> &parents, int count){

	int sum = 0,
		i;
	vector<float> fitnessValues;
	vector<individual*> p;
	
	for(i = 0; i < adults.size(); i++){  //Sum up fitness values
			fitnessValues.push_back(adults[i]->getFitness());
			sum += fitnessValues[i];
		}

	for(i = 0; i < fitnessValues.size(); i++){  //Calculate probabilities
			fitnessValues[i] = fitnessValues[i]/sum;
	}
	p = rouletteWheel(fitnessValues, adults, count);
	parents.insert(parents.end(), p.begin(), p.end());
	
}

void selection::sigmaScaling(vector<individual*> &adults, vector<individual*> &parents, int count){

	int i,
		size = adults.size();
	float std = 0,
		avg,
		sum = 0;
	vector<float> sigmaValues;
	vector<individual*> p;

	for(i = 0; i < size; i++){            //Calc total fitness
		sum += adults[i]->getFitness();
	}
	avg = sum / static_cast<float>(size);
	for(int i = 0; i < size; i++){		//Standard deviation
		std += pow(adults[i]->getFitness()-avg, 2);
	}
	std = sqrt(sum/size);	
	sum = 0;
	for(i = 0; i < size; i++){			//Calculate sigma values (unnormalized)
		sigmaValues.push_back(exp((1+( adults[i]->getFitness()-avg )/(2*std))));
		sum += sigmaValues[i];
	}
	for(i = 0; i < size; i++){         //Normalized probability
		sigmaValues[i] = sigmaValues[i]/sum;		
	}
	p = rouletteWheel(sigmaValues, adults, count);
	parents.insert(parents.end(), p.begin(), p.end());

}
void selection::rankSelection(vector<individual*> &adults, vector<individual*> &parents, int count){
		
	vector<float> rankValues;
	int i,
		size = adults.size(),
		r = size;
	float num = r-1,
		sum = 0.0f;
	vector<individual*> p;
			
	sort(adults.begin(), adults.end(), fitnessSortFunc);
	for(i = 0; i < size; i++){                    //Calculate rank values
		rankValues.push_back(exp((MIN_RANKSELECT_VAL+(MAX_RANKSELECT_VAL-MIN_RANKSELECT_VAL)*(((r-1)/num)))));
		sum += rankValues[i];
		r--;
	}
	for(i = 0; i < size; i++){					 //Calculate normalized probabilities
		rankValues[i] = rankValues[i]/sum;
	}
	p = rouletteWheel(rankValues, adults, count);
	parents.insert(parents.end(), p.begin(), p.end());

}

//Tournament selection function
//The value of e determines the win chance of the fittest phenom
void selection::tournamentSelection(vector<individual*> &adults, vector<individual*> &parents, int count, int elitism){
	
	int random,
		size = adults.size();
	float r;
	vector<individual*> tournament;

	for(int i = 0; i < count; i++){
		for(int j = 0; j < tournamentSize; j++){				 //Fill tournament spots
			random = (rand()%(size-elitism)) + elitism;
			tournament.push_back(adults[random]);   
		}
		sort(tournament.begin(), tournament.end(), fitnessSortFunc); //Sort tournament by fitness value
		r = (float)rand()/RAND_MAX;
		(r < e) ? parents.push_back(tournament[0]) : parents.push_back(tournament[1]);
		tournament.clear();
	}
}

//Roulette wheel for selecting 'populationSize' in the fashion of a roulette wheel with sectors the
//Size of the normalized probabilities which are passed to the function
vector<individual*> selection::rouletteWheel(vector<float> probabilities, vector<individual*> selectFrom, int count){

	vector<individual*> result;
	int probCount = probabilities.size(),
		i,
		j;
	float cumulativeProb, random;

	for(i = 0; i < count; i++){
		cumulativeProb = 0;
		random = (float)rand()/RAND_MAX;
		for(j = 0; j < probCount; j++){
			if(random >= cumulativeProb && random < cumulativeProb+probabilities[j]){
				result.push_back(selectFrom[j]);
				break;
			}
			cumulativeProb += probabilities[j];
		}
	}
	return result;

}

//Function for sorting individuals by fitness
bool selection::fitnessSortFunc(individual* p1, individual* p2){
	return (p1->getFitness() > p2->getFitness());			//Sort in descending order
}

string selection::returnMechanism(){
	if(selectionMechanism == FITNESS)
		return "Fitness-Proportionate";
	else if(selectionMechanism == SIGMA)
		return "Sigma-Scaling";
	else if(selectionMechanism == RANK)
		return "Rank-Selection";
	else if(selectionMechanism == TOURNAMENT)
		return "Tournament Selection";
}

string selection::returnProtocol(){
	if(selectionProtocol == FULLGENREP)
		return "Full Generational Replacement";
	else if(selectionProtocol == OVERPROD)
		return "Over-Production";
	else if(selectionProtocol == GENMIXING)
		return "Genom-Mixing";
}