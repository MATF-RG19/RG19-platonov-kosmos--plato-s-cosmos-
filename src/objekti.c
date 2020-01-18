#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../headers/objekti.h"
#include "../headers/image.h"

const float maks_brizna = 0.1;
const float korak_mete = 3;
float brzina_mete = 0.02;
float parametarPom = 0.1;
Plat_telo prepreke[MAX_TELA];

GLuint wall_texture_name;

//Pomocna funkcija za iscrtavanje kordinatnog sistema
void iscrtaj_ose(){
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(-1, 0, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 1, 0);
        glVertex3f(0, -1, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0,  1);
        glVertex3f(0, 0, -1);

        float coef = 1.0/10;
        for (float i=-1; i < 1; i+= coef) {
            glColor3f(1, 0, 0);
            glVertex3f(i, -0.1*coef, 0);
            glVertex3f(i, +0.1*coef, 0);

            glColor3f(0, 0, 1);
            glVertex3f(-0.1*coef, i, 0);
            glVertex3f(+0.1*coef, i, 0);

            glColor3f(0, 1, 0);
            glVertex3f(-0.1*coef, 0, i);
            glVertex3f(+0.1*coef, 0, i);
        }

    glEnd();
}

//Funkcije za prepreke
Plat_telo napravi_prepreke(){
	Plat_telo tmp;
    	tmp.x =  -10;
    	tmp.z = -5;
        tmp.pogodjena_parametar = 0;
        tmp.is_pogodjena = false;
    	tmp.tip = (rand() % 5) + 1;  //dodaje se 1 jer su tela numerisana od 1 --->TODO
    	return tmp;
}

void nacrtaj_plat_tela(){

	for(int i=0;i<MAX_TELA;i++){
            	glPushMatrix();
            	glTranslatef(prepreke[i].x, 0,prepreke[i].z );
 				glRotatef(parametarPom, 1, 1, 0);
            	switch (prepreke[i].tip){
            		case TETRAEDAR:
            			glPushMatrix();
    						glTranslatef(0,-1,0);
    						glScalef(0.8, 0.8, 0.8);
    						glBegin(GL_TRIANGLE_STRIP); //boje podesene samo radi lakseg iscrtavanja
    							/*glColor3f(1, 0, 0); 
    							glColor3f(0, 0, 1); 
    							glColor3f(1, 0, 0); 
    							glColor3f(0, 0, 1); 
    							glColor3f(1, 0, 0); 
    							glColor3f(0, 0, 1); */

    							glVertex3f(0, 2, 0);
								glVertex3f(-1, 0, 1);
								glVertex3f(1, 0, 1);
								glVertex3f(0, 0, -1.4);
								glVertex3f(0, 2, 0);
								glVertex3f(-1, 0, 1);
    						glEnd();
    					glPopMatrix();
            	        break;
            		case HEKSAEDAR: 
            			glPushMatrix();
            				glutSolidCube(0.9); 
            				printf("%d\t", prepreke[i].tip);
            				break;
            			glPopMatrix();
            		case OKTAEDAR:
            			glPushMatrix();
            				glScalef(0.6, 0.6, 0.6);
            				glutSolidOctahedron(); 
            				printf("%d\t", prepreke[i].tip);
            				break;
            			glPopMatrix();
            		case DODEKAEDAR:
            			glPushMatrix();
            				glScalef(0.6, 0.6, 0.6);
            				glutSolidDodecahedron(); 
            				printf("%d\t", prepreke[i].tip);
            				break;
            			glPopMatrix();
            		case IKOSAEDAR:
            			glPushMatrix();
            				glScalef(0.6, 0.6, 0.6);
            				glutSolidIcosahedron(); 
							printf("%d\t", prepreke[i].tip);
            				break;	
            			glPopMatrix();
            		//default: 
            			//glutWireCube(1);
            			//printf("%d\t", prepreke[i].tip);
            	}
            	glPopMatrix();
            	parametarPom += 0.02;
      		glPopMatrix();
	}
}

//Delovi koda za teksture, preuzeti sa sajta asistenta
void initialise(){
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Postavlja se boja pozadine. */
    glClearColor(0, 0, 0, 0);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, "stars.bmp");

    /* Generisu se identifikatori tekstura. */
    glGenTextures(1, &wall_texture_name);

    glBindTexture(GL_TEXTURE_2D, wall_texture_name);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

//Funkcija za crtanje zida
void nacrtaj_kosmos(){
	const static GLfloat material_emission[] = { 0.15, 0.15, 0.15, 1 };
    	
	GLfloat ambient_coeffs[] = { 0.05375, 0.05, 0.06625, 1 };
    	GLfloat diffuse_coeffs[] = { 0.6, 0.6, 0.6, 1 };
    	GLfloat specular_coeffs[] = {  0.332741, 0.528634, 0.346435, 1 };
	GLfloat shininess = 0.3*128;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);


	glPushMatrix();
		glBegin(GL_QUADS);
		    glNormal3f(0, 0, 1);

		    glTexCoord2f(0 ,1);		glVertex3f(-0.5, 0.5, 0);

		    glTexCoord2f(1 ,1);		glVertex3f(0.5, 0.5, 0);

		    glTexCoord2f(1 ,0);		glVertex3f(0.5, -0.5, 0);

		    glTexCoord2f(0 ,0);		glVertex3f(-0.5, -0.5, 0);
		glEnd();
	glPopMatrix();
}

//Funkcija za ispis teksta
void ispisi_tekst(char * tekst, int x, int y, int sirina_ekrana, int duzina_ekrana){
	glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
   	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//boja teksta
	glColor4f(0.000, 0.749, 1.000, 1.0 );
	glOrtho(0, sirina_ekrana, 0, duzina_ekrana, -1, 1);

	glRasterPos2f(x, y);

	int len= strlen(tekst);
	for (int i = 0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, tekst[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

