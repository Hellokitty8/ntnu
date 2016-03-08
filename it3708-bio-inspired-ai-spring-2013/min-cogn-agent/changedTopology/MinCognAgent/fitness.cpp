#include "fitness.h"
#include "individual.h"
#include "CTRNN.h"
#include <cmath>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\glut.h>

//Global in order to work with glut
int Object_Count;
float Object_Height;
int	Object_Size;
int Object_X_Movement;
int	Time_Counter;
int	Score;
int Player_Movement;
vector<int> Player,	Object, Sensors;

//CTRNN
CTRNN gameController(10);

//Glut Hack
fitness* glutHelper;
void doRender(void)			{ glutHelper->Render(); }
void doIdle()				{ glutHelper->Idle(); }
void doResize(int w, int h) { glutHelper->Resize(w, h); }

#ifdef FORCEDBLOCKDIST
       bool pickedBlocks[NUMBLOCKS] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
#endif 

fitness::fitness(){
	bestFitness = 0;
	catchTreshold = 1.0f;
}

//Fitness testing function
void fitness::testFitness(vector<individual*> generation)
{
	for (int i = 0; i < generation.size(); i++) {
		initializeCTRNN(generation[i]->getPhenom());
		Score = 0;
		gameSimulation();
		if(Score > bestFitness){
			bestFitness = Score;
			bestPhenom = generation[i]->getPhenom();
		}
		generation[i]->setFitness(Score);
	}
	
}

void fitness::visualizeCurrentBest(){
	
	char *myargv [1];
	int myargc=1;
	myargv [0]=strdup ("Minimally Coginitive Agent");
	initializeCTRNN(bestPhenom);
	startAnimation(myargc, myargv);

}


//Set the variables used in the fitness class
void fitness::setFitnessVariables(int genomSize){

}

void fitness::writeBestPhenomToFile(){
	fstream phenoms;
	phenoms.open ("phenoms.txt", fstream::in | fstream::out | fstream::app);
	for(int i = 0; i < bestPhenom.size(); i++)
		phenoms << bestPhenom[i] << "  ";
	phenoms << "\n";
	phenoms.close();

}


void fitness::reset(){
	
	bestFitness = 0;
}

void fitness::gameSimulation(){

	//Initialize Player and First Object
	initializeGame();
	//For each Object Droped
	for(int i = 0; i < OBJECT_DROPS; i++){
		//While Object is falling
		//Object_X_Movement = rand()%3-1;
		while (Object_Height < GAME_HEIGHT-1){
			getSensorData();
			setInputNeurons();
			gameController.EulerStep(integrationStepSize);
			Player_Movement = playerMovement();
			movePlayer(Player_Movement);
			moveObject();
		}
		Score +=  computeDropScore();
		createNewObject();
	}
}

void fitness::initializeGame(){
#ifdef FORCEDBLOCKDIST
        for (int i = 0; i < NUMBLOCKS; i ++) {
                pickedBlocks[i] = false;
        }
#endif 
        Object_Count = 0;
        Player.clear();
        for(int i = 0; i < PLAYER_WIDTH; i++)
                Player.push_back(PLAYER_STARTING_POS+i);
        createNewObject();
}
 


//Move the object one step down
void fitness::createNewObject(){
               
        Object.clear();
        Object_Count++;
        Object_Height = 0;
#ifdef RANDOMBLOCKDIST
        Object_Size = rand()%MAX_OBJECT_WIDTH+1;
#endif
#ifdef FORCEDBLOCKDIST
        // NUMBLOCKS, never go past j to ensure no segfault
        int availableBlocks [NUMBLOCKS];
        int j = 0;
        int savedIndexes [NUMBLOCKS];
        // check which values that already are taken (this makes it run much faster when the number of taken values increases)
        for (int i = 0; i < NUMBLOCKS; i++) {
                if (pickedBlocks[i] == false) {
                        savedIndexes[j] = i;
                        availableBlocks[j++] = forcedBlockDist[i];
                }
        }
 
        int randomIndex;
        if (j == 1)
                randomIndex = 0;
        else if (j == 0)
                randomIndex = 0;
        else
                randomIndex = rand()%j;
        int t = savedIndexes[randomIndex];
        pickedBlocks[t] = true;
        Object_Size = availableBlocks[randomIndex];
#endif
        int x_pos = rand()%(GAME_WIDTH-Object_Size);
        for(int i = 0; i < Object_Size; i++)
                Object.push_back(x_pos + i);
}

//Move the player depending on the input
//When moving out of bounds (Left or Right) the player will appear
//On the opposite side of the screen
void fitness::movePlayer(int x_movement){

	for(int i = 0; i < PLAYER_WIDTH; i++){
		Player[i] += x_movement;
		//If out of bounds RIGHT SIDE
		if(Player[i] > GAME_WIDTH-1)
			Player[i] -= GAME_WIDTH;
		//If out of bounds LEFT SIDE
		else if(Player[i] < 0)
			Player[i] += GAME_WIDTH;
	}
}

//Check which shadow sensors are activated
void fitness::getSensorData(){
	
	bool s;
	Sensors.clear();
	for(int i = 0; i < PLAYER_WIDTH; i++){
		s = false;
		for(int j = 0; j < Object_Size; j++){
			if(Player[i] == Object[j]){
				Sensors.push_back(1);
				s = true;
			}
		}
		if(!s)
			Sensors.push_back(0);
	}
}

//Compute the drop score 
//If the player is overlapped by an equal sized object or a larger one -> penalty
//If the player catches a smaller object (overlap percentage >= treshold) -> bonus
//If the player fails to meet the overlap treshold of smaller object -> no catch penalty
float fitness::computeDropScore(){

	int overlap = 0;
	float overlapPercentage;

	for(int i = 0; i < Object_Size; i++){
		for(int j = 0; j < PLAYER_WIDTH; j++){
			if(Object[i] == Player[j])
				overlap++;
		}
	}

	// defined in fitness.h
#ifdef CATCHALL // special case for task 2
	if (Object_Size >=  PLAYER_WIDTH) {
		if (overlap == PLAYER_WIDTH)
			return CATCH_BONUS;
		else
			return NO_CATCH_PENALTY;
	}
	else {
		if (overlap == Object_Size)
			return CATCH_BONUS;
		else
			return NO_CATCH_PENALTY;
	}
#endif
#ifdef CATCHAVOID // general case.

	if (Object_Size >=  PLAYER_WIDTH) {
		if (overlap > 0)
			return CRUSHED_PENALTY;
		else
			return AVOIDANCE_BONUS;
	}
	else {
		if (overlap == Object_Size)
			return CATCH_BONUS;
		else
			return NO_CATCH_PENALTY;
	}
#endif

}

void fitness::Render()
{
	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window and the depth buffer
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
   
	if(Object_Count < OBJECT_DROPS){

		//[0, 0] = Lower Left Corner
		glTranslatef (-50.0, -50.0, 0.0); 

		//Draw Player - Green
		glColor3f(0.0f, 1.0f, 0.0f);

		for(int i = 0; i < PLAYER_WIDTH; i++){	
			glRectf(Player[i]*GRID_X_SIZE, 0, Player[i]*GRID_X_SIZE + GRID_X_SIZE, GRID_Y_SIZE);
		}
	
		//Draw the Falling Object - Red
		glColor3f(1.0f, 0.0f, 0.0f);			
		glRectf(Object[0]*GRID_X_SIZE,(GAME_HEIGHT-1-Object_Height)*GRID_Y_SIZE ,  (Object[0] + Object_Size)*GRID_X_SIZE, (GAME_HEIGHT - Object_Height)*GRID_Y_SIZE);

	}
	glutSwapBuffers();      
                                
}

void fitness::Resize(int w, int h){ 

	  if (h==0) h=1;
	  glViewport(0,0,w,h);
	  glMatrixMode(GL_PROJECTION); 
	  glLoadIdentity(); 
	  glOrtho (-50.0f, 50.0f, -50.0f, 50.0f, 100.0f, -100.0f);

}

void fitness::moveObject(){
	Object_Height += OBJECT_VERTICAL_SPEED;
	/* Sideways object movement code
	for(int i = 0; i < Object_Size; i++){
		Object[i] += Object_X_Movement;
		if(Object[i] > GAME_WIDTH-1)
			Object[i] -= GAME_WIDTH;
		//If out of bounds LEFT SIDE
		else if(Object[i] < 0)
			Object[i] += GAME_WIDTH;
	}
	*/
}

void fitness::Idle()
{
	//Slows down the animaiton
	if(Time_Counter > TIME_DELAY){
		if(Object_Count < OBJECT_DROPS){
			if(Object_Height < GAME_HEIGHT-1){
				getSensorData();
				setInputNeurons();
				gameController.EulerStep(integrationStepSize);
				Player_Movement = playerMovement();
				movePlayer(Player_Movement);
				moveObject();
			}
			else{
				//Score += computeDropScore();
				createNewObject();
			}
		}
		else{
			//Return to application
			glutLeaveMainLoop();
		}
		Time_Counter = 0;
	}
	else
		Time_Counter++;
	
	glutPostRedisplay();
}

void fitness::Setup(){

	//Ènables us to close the glut window without exiting the program
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	//Parameter handling
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { -50.0, 20.0, 150.0, 1.0 };
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_POINT_SMOOTH);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void fitness::startAnimation(int argc, char* argv[]){

	  initializeGame();
	  glutInit(&argc, argv);
	  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	  glutInitWindowPosition(0,0);
	  glutCreateWindow("MinCogAgent");
	  Setup();
	  glutDisplayFunc(doRender);
	  glutReshapeFunc(doResize);
	  glutIdleFunc(doIdle);
	  glutMainLoop();
} 

void fitness::initializeCTRNN(vector<float> phenom){
	
	//gameController.RandomizeCircuitState(-0.5,0.5);
	gameController.SetCircuitSize(10);

	//Set Connection Weights
	gameController.SetConnectionWeight(1, 6, phenom[0]);
	gameController.SetConnectionWeight(1, 7, phenom[1]);
	gameController.SetConnectionWeight(1, 8, phenom[2]);
	gameController.SetConnectionWeight(2, 6, phenom[3]);
	gameController.SetConnectionWeight(2, 7, phenom[4]);
	gameController.SetConnectionWeight(1, 8, phenom[5]);
	gameController.SetConnectionWeight(3, 6, phenom[6]);
	gameController.SetConnectionWeight(3, 7, phenom[7]);
	gameController.SetConnectionWeight(1, 8, phenom[8]);
	gameController.SetConnectionWeight(4, 6, phenom[9]);
	gameController.SetConnectionWeight(4, 7, phenom[10]);
	gameController.SetConnectionWeight(1, 8, phenom[11]);
	gameController.SetConnectionWeight(5, 6, phenom[12]);
	gameController.SetConnectionWeight(5, 7, phenom[13]);
	gameController.SetConnectionWeight(1, 8, phenom[14]);

	//Hidden Layer -> Hidden Layer
	gameController.SetConnectionWeight(6, 6, phenom[15]);
	gameController.SetConnectionWeight(6, 7, phenom[16]);
	gameController.SetConnectionWeight(6, 6, phenom[17]);
	gameController.SetConnectionWeight(7, 6, phenom[18]);
	gameController.SetConnectionWeight(7, 7, phenom[19]);
	gameController.SetConnectionWeight(7, 8, phenom[20]);
	gameController.SetConnectionWeight(8, 6, phenom[21]);
	gameController.SetConnectionWeight(8, 7, phenom[22]);
	gameController.SetConnectionWeight(8, 8, phenom[23]);
	
	//Hidden Layer -> Motor Output Layer
	gameController.SetConnectionWeight(6, 9, phenom[24]);
	gameController.SetConnectionWeight(6, 10, phenom[25]);
	gameController.SetConnectionWeight(7, 9, phenom[26]);
	gameController.SetConnectionWeight(7, 10, phenom[27]);
	gameController.SetConnectionWeight(8, 9, phenom[28]);
	gameController.SetConnectionWeight(8, 10, phenom[29]);


	//Motor Output Layer -> Motor Output Layer
	gameController.SetConnectionWeight(9, 10, phenom[30]);
	gameController.SetConnectionWeight(9, 10, phenom[31]);
	gameController.SetConnectionWeight(10, 9, phenom[32]);
	gameController.SetConnectionWeight(10, 9, phenom[33]);

	//Set Biases
	gameController.SetNeuronBias(6, phenom[34]);
	gameController.SetNeuronBias(7, phenom[35]);
	gameController.SetNeuronBias(8, phenom[36]);
	gameController.SetNeuronBias(9, phenom[37]);
	gameController.SetNeuronBias(10, phenom[38]);

	//Set Neuron Gains
	gameController.SetNeuronGain(6, phenom[39]);
	gameController.SetNeuronGain(7, phenom[40]);
	gameController.SetNeuronGain(8, phenom[41]);
	gameController.SetNeuronGain(9, phenom[42]);
	gameController.SetNeuronBias(10, phenom[43]);

	//Set Time Constants
	gameController.SetNeuronTimeConstant(6, phenom[44]);
	gameController.SetNeuronTimeConstant(7, phenom[45]);
	gameController.SetNeuronTimeConstant(8, phenom[46]);
	gameController.SetNeuronTimeConstant(9, phenom[47]);
	gameController.SetNeuronTimeConstant(10, phenom[48]);
	
}

void fitness::setInputNeurons(){

	gameController.SetNeuronOutput(1, Sensors[0]);
	gameController.SetNeuronOutput(2, Sensors[1]);
	gameController.SetNeuronOutput(3, Sensors[2]);
	gameController.SetNeuronOutput(4, Sensors[3]);
	gameController.SetNeuronOutput(5, Sensors[4]);
	
}

int fitness::playerMovement(){

	float V = (gameController.NeuronOutput(8) - gameController.NeuronOutput(9))/C;
	V = floor(V + 0.5f);
	return V;

}

void fitness::printCRTNN(){
	
	for(int i = 1; i < 10; i++){
		cout << gameController.NeuronState(i) << " ";

	}

}