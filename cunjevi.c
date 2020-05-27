#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "cunjevi.h"
#include "loptice.h"
#include "sto.h"

float cunj[5][3]; //matrica pozicije cunjeva
int oboren[5];   //niz indikatora da li je cunj oboren
float radiusCone = 0.07;

//odredjivanje pozicije cunjeva
void inicijalizacijaCunjeva() {

    for(int i=0; i<5; i++) {
          cunj[i][0] = 0;
          if(i==0)
            cunj[i][1] = -0.6;
          else
            cunj[i][1] = cunj[i-1][1] + 0.3f;
          cunj[i][2] = 0.13;
          oboren[i] = 0;
    }
}

//iscrtavanje cunjeva
void cunjevi() {

    for(int i=0; i<5; i++) {

       if(oboren[i] == 0) {
          glColor3f(0.5, 0.25, 0.2);
          glPushMatrix();
            glTranslatef(0, 0.385, -cunj[i][1]);
	    glutSolidSphere(0.03,100,100);
          glPopMatrix();
          glPushMatrix();
            glRotatef(-90, 1, 0, 0);
            glTranslatef(cunj[i][0], cunj[i][1], cunj[i][2]);
            glutSolidCone(radiusCone,0.3,100,100);
          glPopMatrix();
	}
    }
}

//funkcija koja odredjuje razdaljina izmedju loptica i cunja
float razdaljinaF(float niz1[3], float niz2[3]) {

     float result0 = (niz2[0] - niz1[0])*(niz2[0] - niz1[0]);
     float result1 = (niz2[2] - niz1[1])*(niz2[2] - niz1[1]);
     float result2 = (niz2[1] + niz1[2])*(niz2[1] + niz1[2]);

     float result = sqrt(result0 + result1 + result2);

     return result;
}

//funkcija koja proverava da li je loptica udarila cunj
bool udarenCunj(float niz1[3], float niz2[3]){

        float pomocni[3];
        for(int i=0; i<3; i++)
            pomocni[i] = niz1[i];
        pomocni[1] -= 0.11;
	float razdaljina = razdaljinaF(pomocni, niz2);
	
	float suma = radiusCone + radius;	

	if (razdaljina < suma) 
		return true;
	else 
		return false;				
}


