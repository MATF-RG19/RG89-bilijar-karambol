#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "sto.h"

GLdouble hight = 0.21;


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

