#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "sto.h"
#include "loptice.h"
#include "cunjevi.h"
#include "image.h"

#define TIMER_ID 0
#define FILENAME0 "pozadine/lose.bmp"
#define FILENAME1 "pozadine/win.bmp"
#define FILENAME2 "pozadine/start.bmp"

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special(int key, int x, int y);
void on_timer(int id);
void initialize(void);

void drawLine();
void napraviPutanju(float x1, float y1, float x2, float y2);
float nadjiKPrekoTacaka(float x1, float y1, float x2, float y2);
float nadjiKPrekoUgla(float k, float tgUgla);
void napraviPutanjuBele(bool zid, int znak, float putanjaBele[20][2], int oznaka);
void napraviPutanju3(bool zid, int znak);

int camera_parameter = 0;
int timer_interval = 40;
int parameter = 0;
int flag = 0;
float move_x = 0;
float cilj_x, kraj_x, cilj_z;
float putanja1[20][2];
float putanja2[20][2];
float putanja3[20][2];
int animation_ongoing = 0;
int animation_parameter1 = 0;
int animation_parameter2 = 0;
int animation_parameter3 = 0;
bool minusPlus = false;
float k;
float novo_k;
int udarena = 0;
float pomocna_x, pomocna_y;
bool linija = true;
float k2,k3;
static GLuint names[3];
int lose = 0, brojac = 0;
int win = 0, oboreni = 0;
int start = 1;

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
    initialize();
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
	   cilj_x = pos[0][0];
	   cilj_z = pos[0][2];
           flag = 1;
	   linija = true;
	   glutPostRedisplay();
           break;
        case 'd':
        case 'D':
	   move_x = 0; 
	   cilj_x = pos[1][0];
	   cilj_z = pos[1][2];
           flag = 2;
	   linija = true;
	   glutPostRedisplay();
           break;
	case 'k':
        case 'K':
           kraj_x = cilj_x+move_x;
	   napraviPutanju(pos[2][0], pos[2][2], kraj_x, cilj_z);
	   break;
	case 's':
        case 'S':
	   linija = false;
           if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(timer_interval, on_timer, TIMER_ID);
            }
            break;
	case 'c':
        case 'C':
           pos[2][0] = 1.35;
           pos[2][1] = 0.24;
	   pos[2][2] = 0; 
	   animation_parameter3 = 0;
           glutPostRedisplay();
	   break;
	case 'a':
        case 'A':
	   oboreni = 0;
	   brojac = 0;
           inicijalizacija();
	   inicijalizacijaCunjeva();
	   glutPostRedisplay();
	   break;
	case 'w':
        case 'W':
	   start = 0;
	   glutPostRedisplay();
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

void on_timer(int id) {
    if (id == TIMER_ID) {
         
        if (animation_parameter3 >= 20 || animation_parameter2 >= 20 || animation_parameter1 >= 20) {
            timer_interval = 40;
	    udarena = 0;
            animation_ongoing = 0;
	    animation_parameter3 = 0;
	    animation_parameter2 = 0;
	    animation_parameter1 = 0;
            return;
        }

        if(udarena == 1) {
	   if(animation_parameter1 >= 20)
              animation_parameter1 = 19;
	   pos[0][0] = putanja1[animation_parameter1][0];
           pos[0][2] = putanja1[animation_parameter1][1];
	   animation_parameter1++;
        }

        if(udarena == 2) {
           if(animation_parameter2 >= 20)
              animation_parameter2 = 19;
	   pos[1][0] = putanja2[animation_parameter2][0];
           pos[1][2] = putanja2[animation_parameter2][1];
	   animation_parameter2++;
        }

        pos[2][0] = putanja3[animation_parameter3][0];
        pos[2][2] = putanja3[animation_parameter3][1];
        animation_parameter3 += 1;
	if(animation_parameter3 >= 20)
              animation_parameter3 = 20;
        
        timer_interval++;

    for(int i=0; i<3; i++) {
       if(i==1 && BottomBorder(pos[i])) {
          napraviPutanjuBele(true,1, putanja2, 1);
	  animation_parameter2 = 0;
       }else if(i==1 && TopBorder(pos[i])) {
	  napraviPutanjuBele(true,1, putanja2, 1);
          animation_parameter2 = 0;
       }else if(i==1 && LeftBorder(pos[i])) {
	  napraviPutanjuBele(true,-1, putanja2, 1);
          animation_parameter2 = 0;
       }else if(i==1 && RightBorder(pos[i])) {
	  napraviPutanjuBele(true,-1, putanja2, 1);
          animation_parameter2 = 0;
       }
       if(i==2 && BottomBorder(pos[i])) {
          napraviPutanju3(true,1);
	  animation_parameter3 = 0;
       }else if(i==2 && TopBorder(pos[i])) {
	  napraviPutanju3(true,1);
          animation_parameter3 = 0;
       }else if(i==2 && LeftBorder(pos[i])) {
	  napraviPutanju3(true,-1);
          animation_parameter3 = 0;
       }else if(i==2 && RightBorder(pos[i])) {
	  napraviPutanju3(true,-1);
          animation_parameter3 = 0;
       }
       if(i==0 && BottomBorder(pos[i])) {
          napraviPutanjuBele(true,1, putanja1, 0);
	  animation_parameter1 = 0;
       }else if(i==0 && TopBorder(pos[i])) {
	  napraviPutanjuBele(true,1, putanja1, 0);
          animation_parameter1 = 0;
       }else if(i==0 && LeftBorder(pos[i])) {
	  napraviPutanjuBele(true,-1, putanja1, 0);
          animation_parameter1 = 0;
       }else if(i==0 && RightBorder(pos[i])) {
	  napraviPutanjuBele(true,-1, putanja1, 0);
          animation_parameter1 = 0;
       }
    }

    if(flag == 1 && isBallHit(pos[0],pos[2])) {
       animation_parameter1 = 0;
       animation_parameter3 = 0;
       udarena = 1;
       napraviPutanjuBele(false,1, putanja1,0);
       napraviPutanju3(false,1);
    }
    
    if(flag == 2 && isBallHit(pos[1],pos[2])) {
       animation_parameter2 = 0;
       animation_parameter3 = 0;
       udarena = 2;
       napraviPutanjuBele(false,1,putanja2,1);
       napraviPutanju3(false,1);
    }

    for(int i=0; i<3; i++) {
      for(int j=0; j<5; j++) {
         if(udarenCunj(pos[i], cunj[j]) && oboren[j] != 1) {
             if(i != 2) {
                oboren[j] = 1;
                oboreni++;
             }
             else {
                brojac++;
                pos[2][0] = 1.35;
                pos[2][1] = 0.24;
	  	pos[2][2] = 0;
                animation_parameter3 = 20;
             }
          }      
      }
    }

    if(brojac == 3) 
       lose = 1;

    if(oboreni == 5) 
       win = 1;
    
  }

    glutPostRedisplay();

    
    if (animation_ongoing) {
        glutTimerFunc(timer_interval, on_timer, TIMER_ID);
    }
   
}

void initialize(void)
{
     Image * image;
    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    /*Inicijalizuje se objekat*/
    image = image_init(0, 0);

    /* Kreira se tekstura. */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(3, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    image_read(image, FILENAME1);
    
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    image_read(image, FILENAME2);
    
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    /* Iskljucuje se tekstura */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat*/
    image_done(image);
    inicijalizacijaCunjeva();
    inicijalizacija();

}

float distance(float x1, float y1, float x2, float y2) {

     float result0 = (x2 - x1)*(x2 - x1);
     float result1 = (y2 - y1)*(y2 - y1);

     float result = sqrt(result0 + result1);

     return result;
}

float nadjiKPrekoTacaka(float x1, float y1, float x2, float y2) {

    float k = (y2-y1)/(x2-x1);
    return k;
}

float nadjiKPrekoUgla(float k, float tgUgla) {

    float novo_k = (tgUgla+k)/(1-k*tgUgla);
    return novo_k;
}

float nadjiY(float x,float x1,float y1,float k) {

    float y;
    y = k*x - k*x1 + y1;
    return y;
}

void napraviPutanjuBele(bool zid, int znak, float putanjaBele[20][2], int oznaka) {

    int i=20;
    int j=1;

    if(move_x == 0) {

        k2 = k;
        if(zid) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        
	if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.04f || move_x == 0.05f) {

        if(k < 0)
          k2 = k + 0.6;
        else if(k > 0)
          k2 = k - 0.6;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.01f || move_x == 0.02f || move_x == 0.03f) {
       
	if(k < 0)
          k2 = k + 0.2;
	else if(k > 0)
          k2 = k - 0.2;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.06f || move_x == 0.07f || move_x == 0.08f) {

        if(k < 0)
          k2 = k + 1.0;
        else if(k > 0)
          k2 = k - 1.0;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x > 0.08f && move_x < 0.18f) {

        if(k < 0)
          k2 = k + 1.2;
        else if(k > 0)
          k2 = k - 1.2;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.04f || move_x == -0.05f) {

        if(k < 0)
          k2 = 0.75;
        if(k > 0)
          k2 = -0.75;
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.01f || move_x == -0.02f || move_x == -0.03f) {

        if(k < 0)
          k2 = 1.25;
        if(k > 0)
          k2 = -1.25;
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.06f || move_x == -0.07f || move_x == -0.08f) {

        if(k < 0)
          k2 = 0.50;
        else if(k > 0)
          k2 = -0.5;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x < -0.08f && move_x > -0.18f) {

        if(k < 0)
          k2 = 0.30;
        else if(k > 0)
          k2 = -0.3;

        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }
}

void napraviPutanju3(bool zid, int znak) {

    int i=20;
    int j=1;
    if(move_x == 0)  {
	 pomocna_x = putanja3[19][0];
         pomocna_y = putanja3[19][1];
         while(i>0) {
            putanja3[20-i][0] = pomocna_x;
	    putanja3[20-i][1] = pomocna_y;
	    i--;
         }
   }else if(move_x == 0.04f || move_x == 0.05f) {

        if(k < 0)
          k3 = 0.75;
        if(k > 0)
          k3 = -0.75;
        if(zid && znak == 1)
          k3 = k3*(-1.0);
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.01f || move_x == 0.02f || move_x == 0.03f) {
        if(k < 0)
          k3 = 1.25;
        if(k > 0)
          k3 = -1.25;

        if(zid && znak == 1)
          k3 = k3*(-1.0);
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.06f || move_x == 0.07f || move_x == 0.08f) {
        if(k < 0)
          k3 = 0.5;
        if(k > 0)
          k3 = -0.5;
        if(zid && znak == 1)
          k3 = k3*(-1.0);
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x > 0.08f && move_x < 0.18f) {

        if(k < 0)
          k3 = 0.3;
        if(k > 0)
          k3 = -0.3;
        if(zid && znak == 1)
          k3 = k3*(-1.0);
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.04f || move_x == -0.05f) {

        if(k < 0)
          k3 = k + 0.7;
        else if(k > 0)
          k3 = k - 0.7;

        if(zid && znak == 1) {
          k3 = k3*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.01f || move_x == -0.02f || move_x == -0.03f) {

        if(k < 0)
          k3 = k + 1.0;
        else if(k > 0)
          k3 = k - 1.0;

        if(zid && znak ==1) {
          k3 = k3*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == -0.06f || move_x == -0.07f || move_x == -0.08f) {
       
	if(k < 0)
          k3 = k + 0.4;
	else if(k > 0)
          k3 = k - 0.4;

        if(zid && znak == 1) {
          k3 = k3*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x < -0.08f && move_x > -0.18f) {
       
	if(k < 0)
          k3 = k + 0.2;
	else if(k > 0)
          k3 = k - 0.2;

        if(zid && znak == 1) {
          k3 = k3*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }
}




void napraviPutanju(float x1, float y1, float x2, float y2) {

    k = nadjiKPrekoTacaka(x1,y1,x2,y2);
    int i=20;
    int j=1;
    float d = fabs(x1-x2);
    float pomeraj = d/i;

    if(distance(x1+0.1,y1,x2,y2) < distance(x1-0.1,y1,x2,y2)) {
        minusPlus = true;
	while(i>0) {
            float x = x1+j*pomeraj;
            float y = nadjiY(x,x1,y1,k);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
            j++;
        } 
    }else {
	minusPlus = false;
        while(i>0) {
            float x = x1-j*pomeraj;
            float y = nadjiY(x,x1,y1,k);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
        }       
    }
   
}

void drawLine() {

        glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex3f(pos[2][0], pos[2][1], pos[2][2]);
                if(flag == 1)
		  glVertex3f(pos[0][0]+move_x, pos[0][1], pos[0][2]);
                else if(flag == 2)
                  glVertex3f(pos[1][0]+move_x, pos[1][1], pos[1][2]);
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

    if(lose != 0) {

       gluLookAt(0, 0, 8,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, names[0]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        
        glEnd();
        
    
        glBindTexture(GL_TEXTURE_2D, 0);
	lose = 0;

    }else if(win != 0) {

       gluLookAt(0, 0, 8,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, names[1]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        
        glEnd();
        
    
        glBindTexture(GL_TEXTURE_2D, 0);
	win = 0;

    } else if(start == 1) {

       gluLookAt(0, 0, 8,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, names[2]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        
        glEnd();
        
    
        glBindTexture(GL_TEXTURE_2D, 0);
	win = 0;

    } else {
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


       draw_legs();
       draw_edges();
       draw_base();
       cunjevi();
       draw_balls();
       if(linija == true) 
          drawLine();
  }
     
    
    glutSwapBuffers();
}
