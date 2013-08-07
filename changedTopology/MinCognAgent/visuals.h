/*

#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "gl\glut.h"   // - An interface and windows management library
#include <vector>

using namespace std;

const int GAME_HEIGHT = 15;
const int GAME_WIDTH = 30;
const int OBJECT_DROPS = 40;
const int MAX_OBJECT_WIDTH = 6;
const int MIN_OBJECT_WIDTH = 1;
const int PLAYER_WIDTH = 5;
const int PLAYER_HEIGHT = 1;
const int MAX_PLAYER_MOVEMENT = 9;
const int AVOIDANCE_BONUS = 1;
const int CRUSHED_PENALTY = -1;
const int CATCH_BONUS = 1;
const int NO_CATCH_PENALTY = 0;
const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 500;
const float GRID_X_SIZE = 100/GAME_WIDTH;
const float GRID_Y_SIZE = 100/GAME_HEIGHT;
const int OBJECT_THICKNESS = 20;

const int TIME_DELAY = 500;
int time_counter = 0;
vector<int> Player, Object;
vector<bool> Sensors;
int Object_Count, Object_Height, Object_Size;
int Score, Player_Movement;


void createNewObject(){
	
	if(!Object.empty()){
		Object.clear();
	}
	Object_Count++;
	Object_Height = 0;
	Object_Size = rand()%MAX_OBJECT_WIDTH+1;
	int x_pos = rand()%(GAME_WIDTH-Object_Size);

	for(int i = 0; i < Object_Size; i++){
		Object.push_back(x_pos + i);
	}

}
void movePlayer(int x_movement){

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
void getSensorData(){

	Sensors.clear();
	for(int i = 0; i < PLAYER_WIDTH; i++){
		for(int j = 0; j < Object_Size; j++){
			if(Player[i] == Object[j])
				Sensors.push_back(true);
		}
		if(!Sensors[i])
			Sensors.push_back(false);
	}

}
void initGameSettings(){

	for(int i = 0; i < PLAYER_WIDTH; i++){
		Player.push_back(12 + i);
	}
	createNewObject();

}

void Render()
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
		glRectf(Object[0],(GAME_HEIGHT-1-Object_Height)*GRID_Y_SIZE ,  Object[0] + Object_Size*GRID_X_SIZE, (GAME_HEIGHT - Object_Height)*GRID_Y_SIZE);
	
	}
	
	glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

void Resize(int w, int h)
{ 
  //if (h==0) h=1;
  //glViewport(0,0,w,h);
  glViewport(0,0,500,500);

  // Setup viewing volume
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
 
  //         L,      R,      B,     T,     N,      F
  glOrtho (-50.0f, 50.0f, -50.0f, 50.0f, 100.0f, -100.0f);
}
void Idle()
{
	if(time_counter > TIME_DELAY){

		if(Object_Count < OBJECT_DROPS){
			if(Object_Height < GAME_HEIGHT-1){
				Object_Height++;
				//getSensorData();
				//Use NN to generate player movement
				Player_Movement = -2;
				movePlayer(Player_Movement);
			}
			else{
				createNewObject();
			}
		}
		time_counter = 0;
	}
	else
		time_counter++;
	
	glutPostRedisplay();
}
void Setup() 
{ 
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


void startAnimation(int argc, char* argv[])
{

	 initGameSettings();

	  // initialize GLUT library state
	  glutInit(&argc, argv);

	  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	  glutInitWindowPosition(0,0);

	  // Create and label the main window
	  glutCreateWindow("MinCogAgent");

	  // Configure various properties of the OpenGL rendering context
	  Setup();

	  // Callbacks for the GL and GLUT events:
	  // The rendering function 
	  glutDisplayFunc(Render);
	  glutReshapeFunc(Resize);
	  glutIdleFunc(Idle);
  
	  //Enter main event handling loop
	  glutMainLoop();
	
}    
*/