// GLUT/Open_GL Single buffer example using some basic primitives in 2D
#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES // Required to access M_PI
#include <math.h> // Required to access sin() and cos()
#include <freeglut.h>
static void display(void)
{
glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glColor3f(0.0, 0.0, 0.0); // Plot 60 black points at random points in a rectangular region
 glPointSize(5.0);
 glBegin(GL_POINTS);
for(int i=0;i<60;i++) glVertex2i(380+(120*rand()/RAND_MAX), 300+(120*rand()/RAND_MAX));
 glEnd();
 glColor3f(1.0, 0.0, 0.0); // Draw a filled triangle (most used primitive in graphics)
 glBegin(GL_TRIANGLES);
 glVertex2i(120, 300);
 glVertex2i(200, 430);
 glVertex2i(280, 300);
 glEnd();
 glColor3f(0.0, 1.0, 0.0); // Draw green sine wave using a line strip
 glLineWidth(3);
 glBegin(GL_LINE_STRIP);
for(int i=0;i<500;i++)
 {
 glVertex2i(70+i, 220+30*sin((double)i*M_PI*4/180.0));
 }
 glEnd();
 glColor3f(0.0, 0.0, 1.0); // Draw blue filled polygons with 3,4,5... sides
float step[]={3,4,5,8,50};
for(int i=0;i<5;i++)
 {
 glBegin(GL_POLYGON);
 for(float angle=0;angle<=360.0;angle+=360.0/step[i]) // Rotate 360 degrees in steps...
 {
 glVertex2f(70+(i*120)+50*cos(angle*M_PI/180.0),90+50*sin(angle*M_PI/180.0));
 }
 glEnd();
 }
 glFlush(); // Send message to cause contents of all buffers to be drawn to screen
}
int _tmain(int argc, char** argv) // Entry point of program
{
 glutInit(&argc, argv); // Start glut
 glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // Create a single buffer with data in RGB format
 glutInitWindowSize(640,480); // Open a window at (10,10) with size (640x480) called
 glutInitWindowPosition(10,10);
 glutCreateWindow("Sprite based game");
 glLoadIdentity();
 glMatrixMode(GL_PROJECTION);
 gluOrtho2D(0.0, 640.0, 0.0, 480.0); // Map OpenGL to Screen crds 1:1, ignore Z, 2D (X,Y)
 glutDisplayFunc(display);
 glutMainLoop(); // Start Glut main loop, exit via break
return 0;
}
