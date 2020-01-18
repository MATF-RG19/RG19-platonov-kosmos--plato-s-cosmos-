#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>

#include "../headers/objekti.h"

static void on_display();
static void on_reshape(int width, int height);
static void on_key_press(unsigned char key, int x, int y);
static void on_key_release(unsigned char key, int x, int y);
static void on_timer(int timer_id);
static void on_timer1(int id);
extern void initialise();

extern GLuint wall_texture_name;

Plat_telo prepreke[MAX_TELA];
int br_pogodjenih_prepreka=0;

/* GLOBALNE PROMENLJIVE */

float pozicija = 0.0f;

bool go_left = false;
bool go_right = false;

int timer_id = 0;
int timer_interval = 15;
bool kraj_simulacije = false;
double parametar1 = 0;

int screen_width = 0;
int screen_height = 0;

float cooldown = 0;

int animation_ongoing = 0;

int main(int argc, char * argv[])
{
	/* Inicijalizuje se GLUT */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	/* Kreira se prozor */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Platonov kosmos");

	/* Registruju se callback funkcije */		
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_release);
    	glutTimerFunc(timer_interval, on_timer, timer_id);

	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1 };
	GLfloat light_specular[] = { 1, 1, 1, 1 };
	
	/* Ambijentalno osvetljenje scene. */ 
	GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_position[] = { 1, 1, 1, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    	glDisable(GL_CULL_FACE);

	/* Normalizacija normala */ 
	glEnable(GL_NORMALIZE);

	srand(time(NULL));   //generisanje seeda za plat. telo

	/* Pozicionira se svijetlo */
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);	
	
	initialise();   

	/* Inicijalna boja poc. menija (midnight blue) */
	glClearColor(0.098, 0.098, 0.439, 0);

	/* Program ulazi u glavnu petlju */
	glutMainLoop();

	return 0;
}

static void on_display()
{
	//Brise se prethodni sadrzaj prozora
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Podesava se tacka pogleda
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1 + parametar1, 0 + parametar1, 0, 0, 0, 0, 1, 0);

	//Prepreke
	nacrtaj_plat_tela();
	
	//Zid
    	glBindTexture(GL_TEXTURE_2D, wall_texture_name);
        glPushMatrix();
            glScalef(300, 300, 1);
            glTranslatef(0, -0.2, -20);
	    glScalef(-1,1,1);
            glTranslatef(0,0,-20);
            nacrtaj_kosmos();
        glPopMatrix();
    	glBindTexture(GL_TEXTURE_2D, 0);

	// Pod
	glBindTexture(GL_TEXTURE_2D, wall_texture_name);
	glPushMatrix();
		glTranslatef(0,-24,0);
		glRotatef(90,1,0,0);
		glScalef(300, 300, 1);
		nacrtaj_kosmos();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//Tekst
	if(animation_ongoing  && parametar1>=2){
    	char str1[255];
    	sprintf(str1, "   Br. pogodaka: %d / %d", br_pogodjenih_prepreka, MAX_TELA);
   		ispisi_tekst(str1, 2, 570, screen_width, screen_height);

   		char str[255];
   		sprintf(str, "Preostalo vreme: 120 s"); //TODO: ovde dodati tajmer
    	ispisi_tekst(str, screen_width - strlen(str1) - 220, 570, screen_width, screen_height);
	}

	if(!animation_ongoing){
		glPushMatrix();
			glScalef(1000, 1000, 1);
			glTranslatef(0, 0, 1);
			nacrtaj_kosmos();
		glPopMatrix();

		char str[255] = "    * PLATONOV KOSMOS *     ";
		ispisi_tekst(str, screen_width/2 - strlen(str) - 135, screen_height/2, screen_width, screen_height);
	}

	// Nova slika se salje na ekran
	glutSwapBuffers();
}

static void on_reshape(int width, int height)
{
	// Podesava se viewport
	glViewport(0, 0, width, height);
    	
	//Cuvaju se vrednosti sirine i visine prozora
	//jer su potrebne u f-jama ispisa
	screen_width = width;
	screen_height = height;

	//Podesavanja projekcije
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float) width / height, 0.01, 1500);
}

//Komande na tastaturi
static void on_key_press(unsigned char key, int x, int y)
{
	switch (key) {
		//Pokretanje simulacije
		case 's':
		case 'S':
			if (!kraj_simulacije) {
		    		animation_ongoing=1;
		    		glutTimerFunc(17, on_timer1, 0);
		    		glutPostRedisplay();
			}
			break;
		//Pauza
		case 'p':
		case 'P':
			animation_ongoing = !animation_ongoing;
			break;
		//Zavrsava se program
		case 27:
			glDeleteTextures(1, &wall_texture_name);
			exit(EXIT_SUCCESS);
			break;
	}
}


static void on_key_release(unsigned char key, int x, int y) {
	//todo
}


//Tajmer
static void on_timer(int id) {
	//Proverava se da li callback dolazi od odgovarajuceg tajmera i kraj simulacije
   	if (id != timer_id || kraj_simulacije)
        	return;

	//Parametar1 za tacku pogleda
    	if(animation_ongoing){
		parametar1 += 0.01;
		if(parametar1 >= 2)
		    parametar1 = 2;
        }


	//Kraj simulacije
    	if (prepreke[MAX_TELA-1].x < -10)
        	kraj_simulacije = true;

	//Forsira se ponovno iscrtavanje prozora
	glutPostRedisplay();
	glutTimerFunc(timer_interval, on_timer, timer_id);
}

//Tajmer za prepreke
static void on_timer1(int id){
	//Proverava se da li callback dolazi od odgovarajuceg tajmera i kraj simulacije
	if(id != 0 || kraj_simulacije)
		return;
	
	if(animation_ongoing)
		glutTimerFunc(17, on_timer1, 0);
}
