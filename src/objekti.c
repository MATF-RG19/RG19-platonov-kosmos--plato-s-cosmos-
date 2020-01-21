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

float paramtar_y = 0.02;
float parametarPom = 0.1;
int polozaj_x_osa = -1; //menja se (alternira) za svako telo 
int param_y_osa = 7;    //parametar je inicijalno postavljen na 7 kako tela 
                        //ne bi odmah pocela da padaju

Telo sva_tela[MAX_TELA];    //niz svih tela

GLuint wall_texture_name;

void iscrtaj_ose(float len) {
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

void inicijalizuj_tela(){	
	for(int i = 0; i < MAX_TELA; i++){
        	sva_tela[i] = napravi_telo();
    }
}

//Funkcija za pravljenje tela
//Vraca strukturu koja opsuje telo (ne nuzno Platonovo)
Telo napravi_telo(){
	Telo t;
        t.x = (rand() % 6) * polozaj_x_osa;
    	t.y = param_y_osa;
    	t.tip = (rand() % 9) + 1;  //dodaje se 1 jer su tela numerisana od 1 
        if(t.tip > 0 && t.tip < 6)
            t.is_platonic = true;
        else
            t.is_platonic = false;     
        
        polozaj_x_osa *= -1;
        param_y_osa += 1;
    	return t;
}

void azuriraj_tela(){
	int i;

	if(paramtar_y < 0.1)
		paramtar_y += 0.00005;

	for (i = 0; i < MAX_TELA; i++) {
        	sva_tela[i].y -= paramtar_y;
	}
}

void nacrtaj_tela(){
	/*GLfloat ambient_coeffs[] = {0.05375, 0.05, 0.06625, 1};
    	GLfloat diffuse_coeffs[] = {0.1, 0.4, 1, 1};
    	GLfloat specular_coeffs[] = {0.332741, 0.528634, 0.346435, 1};
	GLfloat shininess = 0.3*128;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);*/
    
    float tetredar_koef[][3] = {
                                {0.0f, 1.0f, 0.0f}, 
                                {0.943f, -0.333f, 0.0f}, 
                                {-0.471f, -0.333f, 0.8165f}, 
                                {-0.471f, -0.333f, -0.8165f}};

    float materialAmbientColor[][3] = {
                                       {1.0f, 0.5f, 0.0f}, 
                                       {0.0f, 0.5f, 0.5f}, 
                                       {1.0f, 0.0f, 0.0f},
                                       {0.5f, 0.0f, 1.0f}};

    float materialSpecularColor[][3] = {
                                        {1.0f, 0.5f, 0.0f}, 
                                        {1.0f, 0.5f, 0.0f}, 
                                        {1.0f, 0.0f, 0.5f},
                                        {0.5f, 0.1f, 1.0f}};
    GLfloat diffuse_coeffs[] = {0.1, 0.4, 1, 1};

    int points[] = {1, 2, 0, 3, 1}; //za tetredar


	for(int i=0;i<MAX_TELA;i++){
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbientColor[i%4]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecularColor[i%4]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
        //glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecularColor[i]);

        glPushMatrix();
            glTranslatef(sva_tela[i].x, sva_tela[i].y, 0);
 			glRotatef(parametarPom/3, 1, 1, 1);    //objekti se (radi lakseg prepoznavanja) rotiraju po sve 3 ose
            switch (sva_tela[i].tip){      
            	
                case TETRAEDAR:     
                    glPushMatrix();                
                    glScalef(0.8, 0.8, 0.8);
                    for (int i = 0; i < 3 ; i++) {
                        glBegin(GL_TRIANGLE_STRIP);
                            glVertex3fv(tetredar_koef[points[i]]);
                            glVertex3fv(tetredar_koef[points[i + 1]]);
                            glVertex3fv(tetredar_koef[points[i + 2]]);
                        glEnd();
                            }
                        glPopMatrix();
            	    break;

            	case HEKSAEDAR: 
            		glPushMatrix();
            			glutSolidCube(0.4); 
            		glPopMatrix();
            	    break;

            	case OKTAEDAR:
            		glPushMatrix();
            			glScalef(0.4, 0.4, 0.4);
            			glutSolidOctahedron(); 
            		glPopMatrix();
            		break;

            	case DODEKAEDAR:
            		glPushMatrix();
            			glScalef(0.4, 0.4, 0.4);
            			glutSolidDodecahedron(); 
            		glPopMatrix();
            		break;

            	case IKOSAEDAR:
            		glPushMatrix();
            			glScalef(0.4, 0.4, 0.4);
            			glutSolidIcosahedron(); 
            		glPopMatrix();
            		break;	
                case TORUS:
                    glPushMatrix();
                        glScalef(0.4, 0.4, 0.4);
                        glutSolidTorus(0.4,0.8,20,20); 
                    glPopMatrix();
                    break;
                case SFERA:
                    glPushMatrix();
                        glScalef(0.4, 0.4, 0.4);
                        glutSolidSphere(1,50,50); 
                    glPopMatrix();        
                    break;
                case KONUS:
                    glPushMatrix();
                        glScalef(0.4, 0.4, 0.4);
                        glutSolidCone(0.7,1,30,30); 
                    glPopMatrix();
                    break;
                case PLANETA: 
                    glPushMatrix();
                        glScalef(0.4, 0.4, 0.4);
                        glutSolidTorus(0.2,1.5,20,20); 
                        glutSolidSphere(1,50,50); 
                    glPopMatrix();
                    break;
            	default: 
            		glutWireCube(1);
            }
            glPopMatrix();
            parametarPom += 0.02;
        glPopMatrix();
	}
}//f-ja iscrtaj plat telo

void ukloni_telo(char tip_tela){
    switch(tip_tela){
        case 't':
        case 'T':
            for(int i=0; i<MAX_TELA; i++){
                if(sva_tela[i].is_platonic){ //prvi uslov koji treba da vazi je da je telo pravilan poliedar
                    if(sva_tela[i].y > 0 && sva_tela[i].y < 300){ //telo se nalazi u prozoru 
                        if(sva_tela[i].tip == TETRAEDAR){
                            printf("Uklonili ste TETRAEDAR\n");
                            sva_tela[i].y = sva_tela[i].y - 10; //za sada ovako, bice poboljsano
                            break; //cim se nadje prvo telo koje zad. sve uslove, ispada iz petlje
                        }
                    }
                }
            }
        break;

        case 'h':
        case 'H':
            for(int i=0; i<MAX_TELA; i++){
                if(sva_tela[i].is_platonic){ //prvi uslov koji treba da vazi je da je telo pravilan poliedar
                    if(sva_tela[i].y > 0 && sva_tela[i].y < 600){ //telo se nalazi u prozoru 
                        if(sva_tela[i].tip == HEKSAEDAR){
                            printf("Uklonili ste HEKSAEDAR\n");
                            sva_tela[i].y = sva_tela[i].y - 10; //za sada ovako, bice poboljsano
                            break; //cim se nadje prvo telo koje zad. sve uslove, ispada iz petlje
                        }
                    }
                }
            }
        break;
        
        case 'o':
        case 'O':
            for(int i=0; i<MAX_TELA; i++){
                if(sva_tela[i].is_platonic){ //prvi uslov koji treba da vazi je da je telo pravilan poliedar
                    if(sva_tela[i].y > 0 && sva_tela[i].y < 600){ //telo se nalazi u prozoru 
                        if(sva_tela[i].tip == OKTAEDAR){
                            printf("Uklonili ste OKTAEDAR\n");
                            sva_tela[i].y = sva_tela[i].y - 10; //za sada ovako, bice poboljsano
                            break; //cim se nadje prvo telo koje zad. sve uslove, ispada iz petlje
                        }
                    }
                }
            }
        break;

        case 'd':
        case 'D':
            for(int i=0; i<MAX_TELA; i++){
                if(sva_tela[i].is_platonic){ //prvi uslov koji treba da vazi je da je telo pravilan poliedar
                    if(sva_tela[i].y > 0 && sva_tela[i].y < 600){ //telo se nalazi u prozoru 
                        if(sva_tela[i].tip == DODEKAEDAR){
                            printf("Uklonili ste DODEKAEDAR\n");
                            sva_tela[i].y = sva_tela[i].y - 10; //za sada ovako, bice poboljsano
                            break; //cim se nadje prvo telo koje zad. sve uslove, ispada iz petlje
                        }
                    }
                }
            }
        break;      

        case 'i':
        case 'I':
            for(int i=0; i<MAX_TELA; i++){
                if(sva_tela[i].is_platonic){ //prvi uslov koji treba da vazi je da je telo pravilan poliedar
                    if(sva_tela[i].y > 0 && sva_tela[i].y < 600){ //telo se nalazi u prozoru 
                        if(sva_tela[i].tip == IKOSAEDAR){
                            printf("Uklonili ste IKOSAEDAR\n");
                            sva_tela[i].y = sva_tela[i].y - 10; //za sada ovako, bice poboljsano
                            break; //cim se nadje prvo telo koje zad. sve uslove, ispada iz petlje
                        }
                    }
                }
            }
        break;
    }
}

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
    	
	GLfloat ambient_coeffs[] = {0.05375, 0.05, 0.06625, 1};
    GLfloat diffuse_coeffs[] = {0.6, 0.6, 0.6, 1};
    GLfloat specular_coeffs[] = {0.332741, 0.528634, 0.346435, 1};
	GLfloat shininess = 0.3*128;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

	glPushMatrix();
		glBegin(GL_QUADS);
		    glNormal3f(0, 0, 1);

		    glTexCoord2f(0, 15);		glVertex3f(-150, 150, 0);

		    glTexCoord2f(15, 15);		glVertex3f(150, 150, 0);

		    glTexCoord2f(15, 0);		glVertex3f(150, -150, 0);

		    glTexCoord2f(0, 0);		    glVertex3f(-150, -150, 0);
		glEnd();
	glPopMatrix();
}

//Funkcija za ispis teksta
void ispisi_tekst(char * tekst, int x, int y, int sirina_ekrana, int duzina_ekrana)
{
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

	int len = strlen(tekst);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tekst[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

