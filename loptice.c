#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "loptice.h"
#include "sto.h"

float pos[3][3];
GLfloat radius = 0.09;

void inicijalizacija() {

     pos[0][0] = 0.001;
     pos[0][1] = hight + radius - 0.06;
     pos[0][2] = -2.2;

     pos[1][0] = 0;
     pos[1][1] = hight + radius - 0.06;
     pos[1][2] = 2.2;

     pos[2][0] = 1.35;
     pos[2][1] = hight + radius - 0.06;
     pos[2][2] = 0; 
    
}

void draw_balls() {

   for(int i=0; i<3; i++) {
      glPushMatrix();
        glColor3f(1, 1, 1);
	GLfloat black[] = { 0,0,0,1.0 };
        if(i == 2) {
           glColor3f(0, 0, 0);
           glMaterialfv(GL_FRONT, GL_EMISSION, black);
        }
        else
           glMaterialfv(GL_FRONT, GL_EMISSION, black);
        glTranslatef(pos[i][0], pos[i][1], pos[i][2]);
	glutSolidSphere(radius,100,100);
      glPopMatrix();
   }

}

float razdaljinaK(float niz1[3], float niz2[3]) {

     float result0 = (niz2[0] - niz1[0])*(niz2[0] - niz1[0]);
     float result1 = (niz2[1] - niz1[1])*(niz2[1] - niz1[1]);
     float result2 = (niz2[2] - niz1[2])*(niz2[2] - niz1[2]);

     float result = sqrt(result0 + result1 + result2);

     return result;
}

bool isBallHit(float niz1[3], float niz2[3]){
	float razdaljina = razdaljinaK(niz1, niz2);
	
	float suma = 2*radius;	

	if (razdaljina <= suma) 
		return true;
	else 
		return false;				
}


