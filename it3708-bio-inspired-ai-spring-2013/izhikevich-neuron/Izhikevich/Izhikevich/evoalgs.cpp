#include "evoalgs.h"
#include "helper.h"

helper eaHelper;
fitness fitFunctions;
selection selFunctions;
statistics statFunctions;

evoalgs::evoalgs(){

	setEAParameters();
	evolutionCycle();
}

//Main Evolution Loop
void evoalgs::evolutionCycle(){
	
	float maxFitness;
	static char command = 'y';
	bool initialRun = true;
	ofstream myfile;
	
	generateChildren();							//Generate Children
	while(command != 'n'){
		if(!initialRun){
			reset();
			fitFunctions.reset();
		}
		bool solutionFound = false;	
		generationCount = 0;
		myfile.open("earesult.txt");
		cout << "\nRunning the evolutionary algorithm!";
		while(generationCount < generations && !solutionFound){
			evolveChildren();											//Evolve current children
			if(fitFunctions.testFitness(youths))						//Test fitness of juveniles	
				lastProgress = generationCount;
			maxFitness = statFunctions.calculateGenerationStats(youths, generationCount+1, myfile);	
			if(maxFitness >= treshold && treshold != 0)									
				solutionFound = true;
			selFunctions.adultAndParentSelection(youths, adults, parents, populationSize, elitism);		
			reproduction();											//Parent selection and reproduction
			generationCount++;						
		}
		if(solutionFound)
			cout << "\nSolution Found! (" << generationCount-1 << " Generations)";
		myfile.close();
		fitFunctions.writeBestSolutionToFile();
		initialRun = false;
		do{
			cout << "\n\nDo you wish to run another evolution cycle? (y/N): ";
			cin >> command;
		}while(command != 'n' && command != 'y');
	}
}


void evoalgs::setEAParameters(){

	genomSize = 75;//eaHelper.readInt("Genom Size", MIN_GENOM, MAX_GENOM);
	generations = 200;//eaHelper.readInt("Evolution Cycles", MIN_GENERATIONS, MAX_GENERATIONS);
	populationSize = 100;//eaHelper.readInt("Population Size", MIN_POPULATION, MAX_POPULATION);
	treshold = 0; //eaHelper.readInt("Solution Threshold (0 If None)", MIN_TRESHOLD, MAX_TRESHOLD);
	//crossoverPoints = eaHelper.readInt("Crossover Points", MIN_CROSSOVER, MAX_CROSSOVER);
	crossoverRate = eaHelper.readFloat("Chance of Crossover", MIN_CROSSOVER_RATE, MAX_CROSSOVER_RATE);
	mutationPercentage = eaHelper.readFloat("Chance of Mutation", MIN_MUTATION, MAX_MUTATION);
	mutation = eaHelper.readFloat("Maximum Mutation Percentage", MIN_BITWISE_MUTATION, MAX_BITWISE_MUTATION);
	elitism = eaHelper.readInt("Elitism", MIN_ELITISM, MAX_ELITISM);
	selFunctions.setSelectionParameters(populationSize);
	fitFunctions.setFitnessVariables(genomSize);

}

//Reset parameters and the current generation
void evoalgs::reset(){

	char command;
	cout << "Would you like to keep the same settings? (y/N): ";
	cin >> command;
	if(command != 'y')
		setEAParameters();
	deletePopulation();
	generateChildren();

}

void evoalgs::generateChildren(){

	individual* c;
	for(int i = 0; i < populationSize; i++){
		c = new individual(genomSize);
		youths.push_back(c);
	}
}

void evoalgs::evolveChildren(){

	for(int i = 0; i < youths.size(); i++){
		youths[i]->evolve();
	}
}

void evoalgs::reproduction(){

	int r1,
		r2,
		i,
		p = parents.size(),
		mutationBonus;
	float random;
	individual* c;
	vector<individual*>::iterator it;

	for(it = youths.begin(); it < youths.end();){		//Delete old generation of children
			delete *it;
			it = youths.erase(it);			
	}
	for(i = 0; i < elitism; i++){							//If elitism, send the n fittest to the
		c = new individual(parents[i]->getGenomcode());		//Next generation
		youths.push_back(c);
	}
	for(i = elitism; i < (p-1); i+=2){						//For the remaining children to be generated
		random = (float)rand()/RAND_MAX;			   
		if(random < crossoverRate){							//Check if crossover should be performed
			randomCrossover(parents[i], parents[i+1], 2);	
			//crossover(parents[i], parents[i+1], 2);	
		}
		else{												//If no crossover
			c = new individual(parents[i]->getGenomcode());
			youths.push_back(c);
			c = new individual(parents[i+1]->getGenomcode());
			youths.push_back(c);
		}
	}
	if((populationSize-elitism)%2 != 0){							 //If not even number of parents
		random = (float)rand()/RAND_MAX;				 //Generate 1 child with last parent
		if(random < crossoverRate){						 //+ 1 random
			r1 = parents.size()-1;				
			r2 = rand()%populationSize;
			randomCrossover(parents[r1], parents[r2], 1);
			//crossover(parents[r1], parents[r2], 1);	 
		}
		else{
			r1 = parents.size()-1;
			c = new individual(parents[r1]->getGenomcode());
			youths.push_back(c);
		}													
	}
	for(i = elitism; i < youths.size(); i++)  {				
		random = (float)rand()/RAND_MAX;
		if(random < mutationPercentage){
			//    ***	Izhikevich Specific	****
			//If more than K generations since last max Fitness improvement
			//Increase the mutationBonus by a factor of 1 for each K generations
			mutationBonus = floor((generationCount-lastProgress)/(float)STAGNANT_LIMIT + 0.5f);
			mutationBonus = 1 + mutationBonus;
			youths[i]->mutateGenom(mutation*mutationBonus);
		}
	}
}

//Crossover function, takes 2x individuals and the number of crossover points
//Splits the genom into uniform segments and swaps them
void evoalgs::crossover(individual* a, individual* b, int numberOfChildren){
	
	individual* c;
	vector<int> newGenom;
	vector<vector<int> > parents;
	int segmentSize = genomSize/(crossoverPoints+1);
	int diff = genomSize-(segmentSize*(crossoverPoints+1));
	int p, rDiff, sSize, bits = 0;
	parents.push_back(a->getGenomcode());
	parents.push_back(b->getGenomcode());

	for(int i = 0; i < numberOfChildren; i++){
		rDiff = diff;									//Remaining difference to deal with
		bits = 0;										//Reset bit count
		for(int j = 0; j < (crossoverPoints+1); j++){
			sSize = segmentSize;						//Rounded segment size
			if(rDiff != 0){								//If difference remaining (genomSize%segmentSize != 0)
				if(rDiff > 0) { ++sSize; --rDiff; }		//If remainingDiff is positive
				else		  { --sSize; ++rDiff; }		//... is negative
			}
			if(i == 0)	 { p = (j%2==0) ? 0 : 1; }		//Choose segment from right parents
			else		 { p = (j%2==0) ? 1 : 0; }
			newGenom.insert(newGenom.end(), parents[p].begin()+bits, parents[p].begin()+(bits+sSize));
			bits += sSize;
		}
		c = new individual(newGenom);
		youths.push_back(c);
		newGenom.clear();
	}
}

//Crossover function that used one random generated crossover point
void evoalgs::randomCrossover(individual* a, individual* b, int numberOfChildren){
	
	int r, p;
	individual* c;
	vector<int> newGenom;
	vector<vector<int> > parents;
	parents.push_back(a->getGenomcode());
	parents.push_back(b->getGenomcode());
	
	//Izhiekevich Specific!
	//Select a swap point such that variables stay intact instead of an arbitrary point
	r = (rand()%SEGMENT_CROSSOVER_POINTS)*SEGMENT_SIZE;
	
	for(int i = 0; i < numberOfChildren; i++){
		p = (i == 1) ? 0 : 1;
		newGenom.insert(newGenom.end(), parents[i].begin(), parents[i].begin()+r);
		newGenom.insert(newGenom.end(), parents[p].begin()+r, parents[p].end());
		c = new individual(newGenom);
		youths.push_back(c);
		newGenom.clear();
	}
}

//Delete all individuals in the current population
void evoalgs::deletePopulation(){
	
	vector<individual*>::iterator it;

	if(!youths.empty()){
		for(it = youths.begin(); it < youths.end();){
			delete * it;					//Call destructor on removed objects
			it = youths.erase(it);			//Erase from vector
		}
	}
	if(!adults.empty()){
		for(it = adults.begin(); it < adults.end();){
			delete * it;					//Call destructor on removed objects
			it = adults.erase(it);			//Erase from vector
		}
	}
	parents.clear();
}