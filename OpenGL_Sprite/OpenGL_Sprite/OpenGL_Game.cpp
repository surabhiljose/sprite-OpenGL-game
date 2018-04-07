// OpenGL_Game.cpp
// Modified from "Happy Vikings"


//Use Space bar to jump and arrow keys to navigate.



#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>    // Required for sound only (consider OpenAL), 
#include <mmsystem.h>   // Again only required for sound only

#include <freeglut.h>   // GLUT OpenGL Utility Toolkit
#include "Sprite.h"// Prototype Sprite class


// 3 Sprites are used in the "game"
Sprite player;
Sprite ball;
bool ball_moving=true;
bool jumping=false;
bool ascending= true;
bool collission_over=true;
boolean paused=false;
int jumpcount=0;

Sprite background;
Sprite ash;

// Score data and method
int current_score=0;
int high_score=0;
int life_left =5;
char life_text[20];
char score_text[50];
int state=0;


// Callback function called each time screen is to be redrawn (called by OS not you directly)
static void display(void)                                 
{															// Keep game logic out of display()...
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer

		background.drawSprite();							// Draw background at X,Y in a given direction
		ash.drawSprite();                              // Draw ash
		player.drawSprite();								// Draw player	
		ball.drawSprite();								    // Draw ball 
	    
		sprintf(life_text,"life left: %d",life_left);              // Display lifeleft
		Sprite::text(830,420,life_text);
		sprintf(score_text,"high score: %d  current score : %d",high_score,current_score);              // Display text 
		Sprite::text(100,420,score_text);

		glutSwapBuffers();  // Display buffer just drawn and then switch to draw on the other buffer
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
	Sprite::spriteModeOn(0,0);  // (0,0) Causes orginal screen size to be reused
}

// Callback to manage standard keys on the keyboard
//Use spacebar to jump
// In this case 'q' or 'Q' causes program to terminate
static void qwerty_keys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'Q':
    case 'q': glutLeaveMainLoop () ; break;  // (Stop!)
	case ' ':                                 // use space bar to jump
				if(!jumping)
					jumping=true;		
			break;

    default: break;
    }
}





// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
static void function_keys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
			player.advance_frame(1,4);
			player.sprite_x_position-=10;
			player.direction=false;
		break;


    case GLUT_KEY_RIGHT: 
		    player.advance_frame(1,4);
			player.sprite_x_position+=10;
			player.direction=true;
		break;
    default: break;
    }
}

// Game loop
// static void idle()    // if using  glutIdleFunc(idle); this line should replace the one below
static void idle(int v)  // Runs between frame updates in doublebuffer mode, game logic goes here
{
	if(jumping)
	{	
		if(ascending)
		{	player.advance_frame(5,5);
			player.sprite_y_position+=5;
			jumpcount++;
			if(jumpcount==35)
			{
				ascending=false;
			}
		}
		else
		{
			player.advance_frame(6,6);
			player.sprite_y_position-=5;
			jumpcount--;
			if(jumpcount==0)
			{	
				ascending=true;
				jumping=false;
			}
		}

	}
	if(ball_moving)
	{
	  ball.advance_projectile();      // Advance ball
	  if(ball.sprite_y_position<20)   // if it has landed
	  { 	   
		   ball.sprite_y_position=35;
		   ball.sprite_y_velocity=50+(rand()%200);
		   ball.sprite_x_position=200;
		   ball.sprite_x_velocity=50+(rand()%150);
		   ball_moving=false;
		   
	  }
	} 
	
	if (!ball_moving) {ash.advance_frame(1,3); }
	if (ash.frame_no==3)
	{
      ball_moving=true;
	  ash.frame_no=1;
	  current_score+=1;
	}

	ball.advance_frame(0,5);  // Cycle through 6 images [0,5]

	if(player.collision(ball)) // since it will return 'true' multiple times for single pokeball collission; hence we need the below code
								//it will start returning true from the moment pokeball hits pikachu(till the pokeball leaves pickachu) 
		{	if(collission_over)  //collission_over will be true only when the pokeball is completely past pikachu.
			{	
				collission_over=false;
				life_left--; 
			}
		}
	else{
		collission_over=true;
		if(life_left<0)
		{
			if(current_score>high_score) 
			{
			high_score=current_score;}
			current_score=0;
			life_left=5;
		}
	}

    glutPostRedisplay();      // Send message to ask for screen to be redrawn
	glutTimerFunc(10,idle,0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
}

/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	Sprite::spriteModeOn(1024,480);		

	// Start playing background music (uses Windows calls, there are other ways incluing OpenAL)
	PlaySound(L"Music.wav",NULL, SND_LOOP| SND_ASYNC);  

	// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
	// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128
	player.load("Spritesheet.bmp",10,1,320,10,150,150,false);  
	ball.load("Ball6x1.bmp",3,2,100,35,50,50,true);          
	background.load("Background.bmp",1,1,0,0,1024,480,true); 
	ash.load("Ash.bmp",4,1,0,0,210,170,true); 

    glutReshapeFunc(resize);			// Called when window is resized (not required)
	glutDisplayFunc(display);			// Callback function to redraw, an event created by glutPostRedisplay()
    glutKeyboardFunc(qwerty_keys);		// QWERTY Keys
    glutSpecialFunc(function_keys);		// Cursor keys etc 

	//glutIdleFunc(idle);			    // Called during idle time 
	glutTimerFunc(10,idle,0);  // Use timer callback for more consistent timing
	
	
	glutMainLoop();	// Start main loop running (go!)
    
	return EXIT_SUCCESS;
}


