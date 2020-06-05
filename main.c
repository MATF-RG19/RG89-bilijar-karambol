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
int flag = 0;                    // indikator loptice koja je udarena
float move_x = 0;                // promenljiva za odredjivanje cilja udarca loptice     
float cilj_x, kraj_x, cilj_z;    // promenljive koje cuvaju kordinate gde ce loptica udariti
float putanja1[20][2];           // putanja prve loptice
float putanja2[20][2];           // putanja druge loptice
float putanja3[20][2];           // putanja trece(crne) loptice
int animation_ongoing = 0;
int animation_parameter1 = 0;    // parametar kretanje prve loptice
int animation_parameter2 = 0;    // parametar kretanje druge loptice
int animation_parameter3 = 0;    // parametar kretanja trece(crne) loptice
bool minusPlus = false;          // pomocna promenljiva za odredjivanje putanje
float k;                         // koeficijent pravca prave
int udarena = 0;                 // indikator loptice koja je udarena
float pomocna_x, pomocna_y;      // pomocna promenljiva za odredjivanje putanje
bool linija = true;              // idikator za iscrtavanje ciljne linije
float k2,k3;                     // koeficijent pravca prave
static GLuint names[3];          // identifikatori tekstura
int lose = 0, brojac = 0;        // indikator za gameOver prozor
int win = 0, oboreni = 0;        // indikator za win prozor
int start = 1;                   // indikator za startni prozor
int stoji = 1;                   // indikator da loptica stoji 

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
	   // promene ako zelimo da udarimo prvu lopticu
           if (stoji == 1) {
	      move_x = 0;
	      cilj_x = pos[0][0];
	      cilj_z = pos[0][2];
              flag = 1;
	      linija = true;
	      glutPostRedisplay();
           }
           break;
        case 'd':
        case 'D':
	   // promene ako zelimo da udarimo drugu lopticu
	   if (stoji == 1) {
	      move_x = 0; 
	      cilj_x = pos[1][0];
	      cilj_z = pos[1][2];
              flag = 2;
	      linija = true;
	      glutPostRedisplay();
           }
           break;
	case 'k':
        case 'K':
	   // odredjivanje cilja udarca crne loptice
           kraj_x = cilj_x+move_x;
	   napraviPutanju(pos[2][0], pos[2][2], kraj_x, cilj_z);
	   break;
	case 's':
        case 'S':
	   // pokretanje 
           stoji = 0;
	   linija = false;
           if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(timer_interval, on_timer, TIMER_ID);
            }
            break;
	case 'c':
        case 'C':
	   // postavljanje crne loptice za pocetnu poziciju
           pos[2][0] = 1.35;
           pos[2][1] = 0.24;
	   pos[2][2] = 0; 
	   animation_parameter3 = 0;
           glutPostRedisplay();
	   break;
	case 'a':
        case 'A':
	   // postavljanje svih loptica na pocetnu poziciju
	   oboreni = 0;
	   brojac = 0;
           inicijalizacija();
	   inicijalizacijaCunjeva();
	   glutPostRedisplay();
	   break;
	case 'w':
        case 'W':
	   //promena startnog ekrana
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
	     // pomeranje cilja udarca desno
             move_x += 0.01;
	     glutPostRedisplay();
             break;
	case GLUT_KEY_LEFT:
	     // pomeranje cilja udarca levo
             move_x -= 0.01;
	     glutPostRedisplay();
             break;
    }
}

void on_timer(int id) {
    if (id == TIMER_ID) {
         
        // ako je zavrseno kretanje postavljamo parametre na 0
        if (animation_parameter3 >= 20 || animation_parameter2 >= 20 || animation_parameter1 >= 20) {
	    stoji = 1;
            timer_interval = 40;
	    udarena = 0;
            animation_ongoing = 0;
	    animation_parameter3 = 0;
	    animation_parameter2 = 0;
	    animation_parameter1 = 0;
            return;
        }

        // kretanje prve loptice
        if(udarena == 1) {
	   if(animation_parameter1 >= 20)
              animation_parameter1 = 19;
	   pos[0][0] = putanja1[animation_parameter1][0];
           pos[0][2] = putanja1[animation_parameter1][1];
	   animation_parameter1++;
        }

        // kretanje druge loptice
        if(udarena == 2) {
           if(animation_parameter2 >= 20)
              animation_parameter2 = 19;
	   pos[1][0] = putanja2[animation_parameter2][0];
           pos[1][2] = putanja2[animation_parameter2][1];
	   animation_parameter2++;
        }

        // kretanje crne loptice
        pos[2][0] = putanja3[animation_parameter3][0];
        pos[2][2] = putanja3[animation_parameter3][1];
        animation_parameter3 += 1;
	if(animation_parameter3 >= 20)
              animation_parameter3 = 20;
        
        timer_interval++;

    for(int i=0; i<3; i++) {
       // provera da li je druga loptica udarila neku ivicu stola i promena putanje loptice u slucaju udarca
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
       // provera da li je crna loptica udarila neku ivicu stola i promena putanje loptice u slucaju udarca
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
       // provera da li je prva loptica udarila neku ivicu stola i promena putanje loptice u slucaju udarca
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

    // provera da li je crna loptica udarila prvu lopticu i promena putanje loptica u slucaju udarca
    if(isBallHit(pos[0],pos[2])) {
       animation_parameter1 = 0;
       animation_parameter3 = 0;
       udarena = 1;
       napraviPutanjuBele(false,1, putanja1,0);
       napraviPutanju3(false,1);
    }
    
    // provera da li je crna loptica udarila drugu lopticu i promena putanje loptica u slucaju udarca
    if(isBallHit(pos[1],pos[2])) {
       animation_parameter2 = 0;
       animation_parameter3 = 0;
       udarena = 2;
       napraviPutanjuBele(false,1,putanja2,1);
       napraviPutanju3(false,1);
    }

    // provera da li je loptica udarila cunj
    for(int i=0; i<3; i++) {
      for(int j=0; j<5; j++) {
         if(udarenCunj(pos[i], cunj[j]) && oboren[j] != 1) {
             if(i != 2) {
		// ako je bela loptica udarila cunj, uvecavamo broj oborenih cunjeva i oznacavamo da je i-ti cunj oboren
                oboren[j] = 1;
                oboreni++;
             }
             else {
		// ako je crna loptica udarila cunj, postavljamo crnu lopticu na pocetnu poziciju
		// uvecavamo brojac
                brojac++;
                pos[2][0] = 1.35;
                pos[2][1] = 0.24;
	  	pos[2][2] = 0;
                animation_parameter3 = 20;
             }
          }      
      }
    }
   
    // ako je crna loptica udarila 3 puta cunj gubimo
    // postavljamo indikator za gameOver prozor
    if(brojac == 3) 
       lose = 1;

    // ako su oboreni svi cunjevi, pobedili smo
    // postavljamo indikator za win prozor
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

// odredjivanje udaljenosti
float distance(float x1, float y1, float x2, float y2) {

     float result0 = (x2 - x1)*(x2 - x1);
     float result1 = (y2 - y1)*(y2 - y1);

     float result = sqrt(result0 + result1);

     return result;
}

// nalazimo koeficijent pravca prave 
float nadjiKPrekoTacaka(float x1, float y1, float x2, float y2) {

    float k = (y2-y1)/(x2-x1);
    return k;
}

// nalazimo y iz formule: y-y1=((y2-y1)/(x2-x1))*(x-x1)
float nadjiY(float x,float x1,float y1,float k) {

    float y;
    y = k*x - k*x1 + y1;
    return y;
}

// funkcija za odredjivanje putanje bele loptice
void napraviPutanjuBele(bool zid, int znak, float putanjaBele[20][2], int oznaka) {

    int i=20;
    int j=1;

    // putanju odredjujemo u odnosu na promenljivu move_x i u odnosu na promenljivu minusPlus
    if(move_x == 0) {

        // koeficijent pravca ostaje isti
        k2 = k;
	// ako je loptica udarila u zid menjamo koeficijent
        if(zid) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        
	if(!minusPlus) {
           while(i>0) {
	   // smanjujemo kordinau x u odnodu na pocetko x 
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    // povecavamo kordinau x u odnodu na pocetko x
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.04f || move_x == 0.05f) {

	// menjamo koeficijent pravca 
        if(k < 0)
          k2 = k + 0.6;
        else if(k > 0)
          k2 = k - 0.6;

        // ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           while(i>0) {
	   // smanjujemo kordinau x u odnodu na pocetko x
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    // povecavamo kordinau x u odnodu na pocetko x
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if(move_x == 0.01f || move_x == 0.02f || move_x == 0.03f) {
       
        // menjamo koeficijent pravca
	if(k < 0)
          k2 = k + 0.2;
	else if(k > 0)
          k2 = k - 0.2;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
           while(i>0) {
	    // smanjujemo kordinau x u odnodu na pocetko x
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  while(i>0) {
	    // povecavamo kordinau x u odnodu na pocetko x
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if((move_x > 0.05f && move_x < 0.07f) || move_x == 0.07f || move_x == 0.08f) {

        // menjamo koeficijent pravca
        if(k < 0)
          k2 = k + 1.0;
        else if(k > 0)
          k2 = k - 1.0;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // povecavamo kordinau x u odnodu na pocetko x
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

        // menjamo koeficijent pravca
        if(k < 0)
          k2 = k + 1.2;
        else if(k > 0)
          k2 = k - 1.2;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
           // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // povecavamo kordinau x u odnodu na pocetko x
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

        // menjamo koeficijent pravca
        if(k < 0)
          k2 = 0.75;
        if(k > 0)
          k2 = -0.75;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // smanjujemo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k2 = 1.25;
        if(k > 0)
          k2 = -1.25;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // smanjujemo kordinau x u odnodu na pocetko x
	  while(i>0) {
	    float x = pos[oznaka][0]-j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if((move_x > -0.07f && move_x < -0.05) || move_x == -0.07f || move_x == -0.08f) {

	// menjamo koeficijent pravca
        if(k < 0)
          k2 = 0.50;
        else if(k > 0)
          k2 = -0.5;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // smanjujemo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k2 = 0.30;
        else if(k > 0)
          k2 = -0.3;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k2 = k2*(-1.0);
          zid = false;
        }
        if(!minusPlus) {
           // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[oznaka][0]+j*0.07*znak;
            float y = nadjiY(x,pos[oznaka][0],pos[oznaka][2],k2);
            putanjaBele[20-i][0] = x;
            putanjaBele[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // smanjujemo kordinau x u odnodu na pocetko x
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

//funkcija za odredjivanje putanje crne loptice
void napraviPutanju3(bool zid, int znak) {

    int i=20;
    int j=1;

    // putanju odredjujemo u odnosu na promenljivu move_x i u odnosu na promenljivu minusPlus
    if(move_x == 0)  {
         // loptica ne menja poziciju
	 pomocna_x = putanja3[19][0];
         pomocna_y = putanja3[19][1];
         while(i>0) {
            putanja3[20-i][0] = pomocna_x;
	    putanja3[20-i][1] = pomocna_y;
	    i--;
         }
   }else if(move_x == 0.04f || move_x == 0.05f) {

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = 0.75;
        if(k > 0)
          k3 = -0.75;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1)
          k3 = k3*(-1.0);

        if(!minusPlus) {
	   // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
          // smanjujemo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = 1.25;
        if(k > 0)
          k3 = -1.25;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1)
          k3 = k3*(-1.0);

        if(!minusPlus) {
           // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // smanjujemo kordinau x u odnodu na pocetko x
	  while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if((move_x > 0.05f && move_x < 0.07f) || move_x == 0.07f || move_x == 0.08f) {

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = 0.5;
        if(k > 0)
          k3 = -0.5;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1)
          k3 = k3*(-1.0);

        if(!minusPlus) {
	   // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // smanjujemo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = 0.3;
        if(k > 0)
          k3 = -0.3;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1)
          k3 = k3*(-1.0);

        if(!minusPlus) {
	   // povecavamo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // smanjujemo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = k + 0.7;
        else if(k > 0)
          k3 = k - 0.7;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) 
          k3 = k3*(-1.0);
	 
        if(!minusPlus) {
	   // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // povecavamo kordinau x u odnodu na pocetko x
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

	// menjamo koeficijent pravca
        if(k < 0)
          k3 = k + 1.0;
        else if(k > 0)
          k3 = k - 1.0;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak ==1) {
          k3 = k3*(-1.0);
	  zid = false;
        }
        if(!minusPlus) {
	   // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // povecavamo kordinau x u odnodu na pocetko x
	  while(i>0) {
	    float x = pos[2][0]+j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }
    }else if((move_x > -0.07f && move_x < -0.05) || move_x == -0.07f || move_x == -0.08f) {
       
	// menjamo koeficijent pravca
	if(k < 0)
          k3 = k + 0.4;
	else if(k > 0)
          k3 = k - 0.4;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k3 = k3*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
	   // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // povecavamo kordinau x u odnodu na pocetko x
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
       
	// menjamo koeficijent pravca
	if(k < 0)
          k3 = k + 0.2;
	else if(k > 0)
          k3 = k - 0.2;

	// ako je loptica udarila u gornju ili donju ivicu stola menjamo koeficijent
        if(zid && znak == 1) {
          k3 = k3*(-1.0);
          zid = false;
        }

        if(!minusPlus) {
	   // smanjujemo kordinau x u odnodu na pocetko x
           while(i>0) {
	    float x = pos[2][0]-j*0.07*znak;
            float y = nadjiY(x,pos[2][0],pos[2][2],k3);
            putanja3[20-i][0] = x;
            putanja3[20-i][1] = y;
            i--;
	    j++;
          }
        }else if(minusPlus) {
	  // povecavamo kordinau x u odnodu na pocetko x
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


//funkcija za odredjivanje putanje crne loptice
void napraviPutanju(float x1, float y1, float x2, float y2) {

    k = nadjiKPrekoTacaka(x1,y1,x2,y2);
    int i=20;
    int j=1;
    float d = fabs(x1-x2);
    float pomeraj = d/i;

    // proveravamo kad smo blize cilju
    if(distance(x1+0.1,y1,x2,y2) < distance(x1-0.1,y1,x2,y2)) {
	// povecavamo x
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
	// smanjujemo x
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

// funkcija za iscrtavanje putanje 
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

    // ekran ako smo izgubili
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

    // ekran ako smo pobedili
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

    // pocetni ekran
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
       // razlicite pozicije kamere
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

       // funkcije za iscrtavanje
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
