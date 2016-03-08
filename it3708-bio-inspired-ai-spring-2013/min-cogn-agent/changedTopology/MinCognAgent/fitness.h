#ifndef FITNESS_H_
#define FITNESS_H_

#include <vector>

using namespace std;
class individual; 

const int GAME_HEIGHT = 15;
const int GAME_WIDTH = 30;
const int OBJECT_DROPS = 41;
const float OBJECT_VERTICAL_SPEED = 1.0f;
const int MAX_OBJECT_WIDTH = 6;
const int MIN_OBJECT_WIDTH = 1;
const int PLAYER_WIDTH = 5;
const int PLAYER_HEIGHT = 1;
const int PLAYER_STARTING_POS = 13;
const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 500;
const float GRID_X_SIZE = 100/GAME_WIDTH;
const float GRID_Y_SIZE = 100/GAME_HEIGHT;
const float AVOIDANCE_BONUS = 2.0f;
const float CRUSHED_PENALTY = 0.0f;
const float CATCH_BONUS = 2.0f;
const float NO_CATCH_PENALTY = 0.0f;
const int TIME_DELAY = 500;	//Increase to slow down animation speed
const float C = 0.25;
const float integrationStepSize = 1.0f;

#define FORCEDBLOCKDIST
//#define RANDOMBLOCKDIST
#define NUMBLOCKS 42
#ifdef FORCEDBLOCKDIST
	const int forcedBlockDist [NUMBLOCKS] = {1, 6, 5, 5, 2, 2, 6, 4, 1, 3, 4, 2, 6, 1, 6, 6, 4, 2, 1, 3, 2, 5, 1, 5, 4, 2, 3, 3, 5, 3, 5, 1, 2, 4, 3, 6, 6, 1, 4, 5, 3, 4};
#endif
//#define CATCHALL
#define CATCHAVOID

class fitness{
	private:
		float catchTreshold;
		float bestFitness;
		vector<float> bestPhenom;

	public:
		fitness();
		void setFitnessVariables(int genomSize);
		void testFitness(vector<individual*> generation);
		void reset();
		//Problem Specific Functions		
		void gameSimulation();
		float computeDropScore();
		void createNewObject();
		void initializeGame();
		void moveObject();
		void movePlayer(int x_movement);
		void getSensorData();
		void writeBestPhenomToFile();
		//Graphics Functions
		void visualizeCurrentBest();
		void startAnimation(int argc, char* argv[]);
		void Setup();
		void Render();
		void Resize(int w, int h);
		void Idle();
		//CTRNN Functions
		void initializeCTRNN(vector<float> phenom);
		void setInputNeurons();
		int playerMovement();
		void printCRTNN();
};

#endif