#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "loptice.h"
#include "sto.h"

GLfloat pos1[3];
GLfloat pos2[3];
GLfloat pos3[3];
GLfloat radius = 0.09;

void inicijalizacija() {

     pos1[0] = 0;
     pos1[1] = hight + radius - 0.06;
     pos1[2] = -2.2;

     pos2[0] = 0;
     pos2[1] = hight + radius - 0.06;
     pos2[2] = 2.2;

     pos3[0] = 1.35;
     pos3[1] = hight + radius - 0.06;
     pos3[2] = 0;   
}

void draw_balls() {

    //1.
    glPushMatrix();
        glColor3f(1, 1, 1);
        GLfloat white[] = { 1,1,1,1.0 };
        glMaterialfv(GL_FRONT, GL_EMISSION, white);
        glTranslatef(pos1[0], pos1[1], pos1[2]);
	glutSolidSphere(radius,100,100);
    glPopMatrix();

    //2.
    glPushMatrix();
	glColor3f(1, 1, 1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
        glTranslatef(pos2[0], pos2[1], pos2[2]);
	glutSolidSphere(radius,100,100);
    glPopMatrix();

    //3.
    glPushMatrix();
	glColor3f(0, 0, 0);
 	GLfloat black[] = { 0,0,0,1.0 };
        glMaterialfv(GL_FRONT, GL_EMISSION, black);
        glTranslatef(pos3[0], pos3[1], pos3[2]);
	glutSolidSphere(radius,100,100);
    glPopMatrix();

}

