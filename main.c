#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "sto.h"
#include "loptice.h"
#include "cunjevi.h"


static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special(int key, int x, int y);

void drawLine();

int camera_parameter = 0;
int parameter = 0;
int flag = 0;
float move_x = 0;
float cilj_x, kraj_x;



int main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_special);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_position[] = {-1, 3, 2, 1};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);


    glEnable(GL_COLOR_MATERIAL);

    glClearColor(1,1,0.8,0);
    glutMainLoop();

    return 0;
}


void draw_axes(float len) {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();

    glEnable(GL_LIGHTING);
}

void on_keyboard(unsigned char key, int x, int y) {
    switch(key) {
        
        case 'v':
        case 'V':
           camera_parameter =(camera_parameter+1)%4;
        glutPostRedisplay();
        break;
	case 'p':
        case 'P':
	   move_x = 0;
	   cilj_x = pos1[0];
           flag = 1;
	   glutPostRedisplay();
           break;
        case 'd':
        case 'D':
	   move_x = 0; 
	   cilj_x = pos2[0];
           flag = 2;
	   glutPostRedisplay();
           break;
	case 'k':
        case 'K':
           kraj_x = cilj_x+move_x;
	   break;
        case 27:
          exit(0);
          break;
    }
}

void on_special(int key, int x, int y) {
    switch(key) {
     
	case GLUT_KEY_RIGHT:
             move_x += 0.01;
	     glutPostRedisplay();
             break;
	case GLUT_KEY_LEFT:
             move_x -= 0.01;
	     glutPostRedisplay();
             break;
    }
}

void drawLine() {

        glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex3f(pos3[0], pos3[1], pos3[2]);
                if(flag == 1)
		  glVertex3f(pos1[0]+move_x, pos1[1], pos1[2]);
                else if(flag == 2)
                  glVertex3f(pos2[0]+move_x, pos2[1], pos2[2]);
	glEnd();
        glDisable(GL_LINE_STIPPLE);
}



void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 20);
}


void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(camera_parameter == 0) {
       gluLookAt(6, 4, 12,
                 0, 0, 0,
                 0, 1, 0);
    }else if(camera_parameter == 1) {
       gluLookAt(0, 10, 0,
                 0, 0, 0,
                 1, 0, 0);
    }else if(camera_parameter == 2){
       gluLookAt(0, 2, 12,
                 0, 0, 0,
                 0, 1, 0);
    }else {
       gluLookAt(11.5, 3, 0,
                 0, 0, 0,
                 0, 1, 0);
    }

    draw_axes(5);

    if(parameter == 0)
       inicijalizacija();

    draw_legs();
    draw_edges();
    draw_base();
    cunjevi();
    draw_balls();
    drawLine();
    
    glutSwapBuffers();
}

