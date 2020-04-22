#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "cunjevi.h"

void cunjevi() {
    
    //1.
    glColor3f(0.5, 0.25, 0.2);
    glPushMatrix();
        glTranslatef(0, 0.26, 0.6);
	glutSolidSphere(0.03,100,100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, -0.6, 0);
        glutSolidCone(0.07,0.3,100,100);
    glPopMatrix();

    //2.
    glPushMatrix();
        glTranslatef(0, 0.26, 0.3);
	glutSolidSphere(0.03,100,100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, -0.3, 0);
        glutSolidCone(0.07,0.3,100,100);
    glPopMatrix();
  
    //3.
    glPushMatrix();
        glTranslatef(0, 0.26, 0);
	glutSolidSphere(0.03,100,100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(0.07,0.3,100,100);
    glPopMatrix();

    //4.
    glPushMatrix();
        glTranslatef(0, 0.26, -0.6);
	glutSolidSphere(0.03,100,100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.6, 0);
        glutSolidCone(0.07,0.3,100,100);
    glPopMatrix();

    //5.
    glPushMatrix();
        glTranslatef(0, 0.26, -0.3);
	glutSolidSphere(0.03,100,100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.3, 0);
        glutSolidCone(0.07,0.3,100,100);
    glPopMatrix();
}


