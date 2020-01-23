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
static void on_timer(int timer_id);
static void on_timer1(int id);
extern void initialise();

/* GLOBALNE PROMENLJIVE */

extern GLuint wall_texture_name;

Telo sva_tela[MAX_TELA]; //niz svih tela

float pozicija = 0.0f;

int broj_pogodjenih = 0;

int timer_id = 0;
int timer_interval = 15;
bool istek_vremena = false;

double parametar1 = 0;
double parametar2 = 0; //koriscen za ogranicavanje vremena igranja
int animation_ongoing = 0;

int screen_width = 0;
int screen_height = 0;


int main(int argc, char * argv[]){
	/* Inicijalizuje se GLUT */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	/* Kreira se prozor */
	glutInitWindowSize(850, 600);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Platonov kosmos");

	/* Registruju se callback funkcije */		
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_key_press);
    glutTimerFunc(timer_interval, on_timer, timer_id);

	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
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

	inicijalizuj_tela();

	glClearColor(0, 0, 0, 0);

	/* Program ulazi u glavnu petlju */
	glutMainLoop();

	return 0;
}

static void on_display(){
	//Brise se prethodni sadrzaj prozora
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Podesava se tacka pogleda
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1, 7 + parametar1,
			  0, 0, 0, 
			  0, 1, 0);

	//iscrtaj_ose(5);

	//Iscrtavanje platonovih tela
	nacrtaj_tela();
	
	//Iscrtavanje kosmosa
    glBindTexture(GL_TEXTURE_2D, wall_texture_name);
    glPushMatrix();
        glTranslatef(0, -0.2, -20);
	 	glScalef(-1,1,1);
        glTranslatef(0,0,-20);
        nacrtaj_kosmos();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    //Donji deo (kosmosa)
	glBindTexture(GL_TEXTURE_2D, wall_texture_name);
	glPushMatrix();
		glTranslatef(0,-24,0);
		glRotatef(90,1,0,0);
		nacrtaj_kosmos();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	
	//Tekst tokom igre
	if(animation_ongoing  && parametar1>=2){
    	char str1[255];
    	sprintf(str1, "   Br. pogodaka: %d ", broj_pogodjenih);
   		ispisi_tekst(str1, 2, 570, screen_width, screen_height);


   		char str[255];
   		sprintf(str, "Preostalo vreme: %.2fs", 60 - parametar2);	//60 sekundi za igru 
    	ispisi_tekst(str, screen_width - strlen(str1) - 220, 570, screen_width, screen_height);
    	
    	if (istek_vremena==false && broj_pogodjenih==20){
			glPushMatrix();
				glScalef(100, 100, 1);
				glTranslatef(0, 0, 1);
				nacrtaj_kosmos(); //iscrtavanje panela za obavestenje o pobedi
			glPopMatrix();

			
			char str2[255];
			sprintf(str2, "Pobedio si! Pogodjeno: %d", broj_pogodjenih);
			ispisi_tekst(str2, screen_width/2 - strlen(str1) - 120, screen_height/2-5, screen_width, screen_height);
			ispisi_tekst("          ESC - izlaz ", screen_width/2 - strlen(str1) - 120, screen_height/2-40, screen_width, screen_height);
		}
		else if(istek_vremena){	
        	char str2[255];
        	sprintf(str2, "Vreme je isteklo, pogodjeno tela: %d", broj_pogodjenih);
			ispisi_tekst(str2, screen_width/2 - strlen(str1) - 120, screen_height/2-5, screen_width, screen_height);
		}
	}

	if(!animation_ongoing){
		glPushMatrix();
			glScalef(1000, 1000, 1);
			glTranslatef(0, 0, 1);
			nacrtaj_kosmos();
		glPopMatrix();

		/*Ispisivanje teksta na pocetnom ekranu: Naslov i komande*/

		char naslov[255] = "    *** PLATONOV KOSMOS ***";
		ispisi_tekst(naslov, screen_width/2 - strlen(naslov) - 135, screen_height/2+150, screen_width, screen_height);

		char komande[255] = "Komande:";
		ispisi_tekst(komande, screen_width/2 - strlen(komande) - 150, screen_height/2+35, screen_width, screen_height);

		char str1[255] = "S/s - Pokretanje igrice";
		ispisi_tekst(str1, screen_width/2 - strlen(str1) - 135, screen_height/2, screen_width, screen_height);

		char str2[255] = "P/p - Pauza";
		ispisi_tekst(str2, screen_width/2 - strlen(str1) - 135, screen_height/2-30, screen_width, screen_height);

		char str3[255] = "T/t  - Uklanjanje tetraedra";
		ispisi_tekst(str3, screen_width/2 - strlen(str1) - 135, screen_height/2-80, screen_width, screen_height);

		char str4[255] = "H/h - Uklanjanje heksaedra";
		ispisi_tekst(str4, screen_width/2 - strlen(str1) - 135, screen_height/2-110, screen_width, screen_height);

		char str5[255] = "O/o - Uklanjanje oktaedra";
		ispisi_tekst(str5, screen_width/2 - strlen(str1) - 135, screen_height/2-140, screen_width, screen_height);

		char str6[255] = "D/d - Uklanjanje dodekaedra";
		ispisi_tekst(str6, screen_width/2 - strlen(str1) - 135, screen_height/2-170, screen_width, screen_height);
		
		char str7[255] = "I/i  - Uklanjanje ikosaedra";
		ispisi_tekst(str7, screen_width/2 - strlen(str1) - 135, screen_height/2-200, screen_width, screen_height);
	}

	// Nova slika se salje na ekran
	glutSwapBuffers();
}

static void on_reshape(int width, int height){
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
static void on_key_press(unsigned char key, int x, int y){
	switch (key) {
		
		//Pokretanje
		case 's':
		case 'S':
			if (!istek_vremena) {
		    		animation_ongoing=1;
		    		glutTimerFunc(18, on_timer1, 0);
		    		glutPostRedisplay();
			}
			break;
		//Pauza
		case 'p':
		case 'P':
			animation_ongoing = !animation_ongoing;
			break;

		//Uklanjanje tetraedra
		case 't':
		case 'T':
			if(ukloni_telo('t'))
				broj_pogodjenih++;
			break;

		//Uklanjanje heksaedra
		case 'h':
		case 'H':
			if(ukloni_telo('h'))
				broj_pogodjenih++;
			break;

		//Uklanjanje oktaedra
		case 'o':
		case 'O':
			if(ukloni_telo('o'))
				broj_pogodjenih++;
			break;

		//Uklanjanje dodekaedra
		case 'd':
		case 'D':
			if(ukloni_telo('d'))
				broj_pogodjenih++;
			break;

		//Uklanjanje ikosaedra
		case 'i':
		case 'I':
			if(ukloni_telo('i'))
				broj_pogodjenih++;
			break;

		//Zavrsava se program
		case 27:
			glDeleteTextures(1, &wall_texture_name);
			exit(EXIT_SUCCESS);
			break;
	}
}

//Tajmer
static void on_timer(int id) {
	//Proverava se da li callback dolazi od odgovarajuceg tajmera i kraj simulacije
   	if (id != timer_id || istek_vremena)
        	return;

        
	//Parametar1 za tacku pogleda
    	if(animation_ongoing){
			parametar1 += 0.04;
			
			if(parametar1 >= 2){
		    	parametar1 = 2;
				parametar2 += 0.03; /*Tek kada se sve postavi na scenu i zavrsi pocetna
									animacija, krece tajmer za igru.
									Kako se ovaj tajmer (tj. f-ja on_timer) poziva na 
									svakih 15ms, to se za 1 sekundu pozove 66.6 puta. 
									Parametar2 je podesen tako da za 1 sekundu dostigne 
									vrednost 1.*/ 
		    	if(parametar2>=60){	//nakon 60 sekundi, igra se prekida
		    		parametar2=60;
		    		istek_vremena=true;
		    	}
			}
        }

	//Forsira se ponovno iscrtavanje prozora
	glutPostRedisplay();
	glutTimerFunc(timer_interval, on_timer, timer_id);
}

//Tajmer za tela
static void on_timer1(int id){

	//Proverava se da li callback dolazi od odgovarajuceg tajmera i kraj simulacije
	if(id != 0 || istek_vremena)
		return;

	azuriraj_tela();
	
	if(animation_ongoing)
		glutTimerFunc(18, on_timer1, 0);
}