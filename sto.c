#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "sto.h"
#include "loptice.h"

GLdouble hight = 0.21;
//ivice stola
float left = -1.75, right = 1.75;
float top = -2.625, bottom = 2.625;

//funkcija za iscrtavanje osnove stola
void draw_base(){
    glColor3f(0, 0.48, 0);
    glPushMatrix();
      GLfloat green[] = { 0, 0.1, 0,1 };
      glMaterialfv(GL_FRONT, GL_EMISSION, green);
      glRotatef(90,0,1,0);
      glScalef(3, 0.15, 2);
      glutSolidCube(1.75);
    glPopMatrix();
}

//funkcija za iscrtavanje ivice stola
void draw_edges() {

    //1.
    glPushMatrix();
       glTranslatef(0, 0, 2.8);
       glScalef(2.30, 0.28, 0.22);
       glutSolidCube(1.5);
    glPopMatrix();

    //2.
    glPushMatrix();  
       glTranslatef(0, 0, -2.8);
       glScalef(2.30, 0.28, 0.22);
       glutSolidCube(1.5);
    glPopMatrix();

    //3.
    glPushMatrix();
       glTranslatef(1.78, 0, 0);
       glRotatef(90, 0, 1.0, 0);
       glScalef(3.95, 0.28, 0.22);
       glutSolidCube(1.5);
    glPopMatrix();

    //4.
    glPushMatrix();
       glTranslatef(-1.78, 0, 0);
       glRotatef(90, 0, 1.0, 0);
       glScalef(3.95, 0.28, 0.22);
       glutSolidCube(1.5);
    glPopMatrix();
}

//funkcija za iscrtavanje donjeg dela stola
void draw_legs(){

    GLUquadric* quad = gluNewQuadric();
    //1.
    glPushMatrix();
      glTranslatef(-1.45, 0, -2.5);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3f(0.5,0.25,0.2);
      gluCylinder(quad, hight, hight, 1.5, 10, 10);
    glPopMatrix();

    //2.
    glPushMatrix();
      glTranslatef(1.45, 0.0, 2.5);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3f(0.5,0.25,0.2);
      gluCylinder(quad, hight, hight, 1.5, 10, 10);
    glPopMatrix();

    //3.
    glPushMatrix();
      glTranslatef(-1.45, 0.0, 2.5);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3f(0.5,0.25,0.2);
      gluCylinder(quad, hight, hight, 1.5, 10, 10);
    glPopMatrix();

    //4.
    glPushMatrix();
      glTranslatef(1.45, 0.0, -2.5);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3f(0.5, 0.25, 0.2);
      gluCylinder(quad, hight, hight, 1.5, 10, 10);
    glPopMatrix();
}

//funkcija koja proverava da li je loptica udarila levu ivicu stola
bool LeftBorder(float niz[3]){
	if ( niz[0] < (left + 2*radius) ) 
		return true;
	else 
		return false;
}

//funkcija koja proverava da li je loptica udarila desnu ivicu stola
bool RightBorder(float niz[3]){
	if (niz[0] > (right - 2*radius))
		return true;
	else
		return false;
}

//funkcija koja proverava da li je loptica udarila gornju ivicu stola
bool TopBorder(float niz[3]){
	if ( niz[2] < (top + 2*radius) )
		return true;
	else 
		return false;
}

//funkcija koja proverava da li je loptica udarila donju ivicu stola
bool BottomBorder(float niz[3]){
	if ( niz[2] > (bottom - 2*radius) )
		return true;
	else 
		return false;	
}

