#ifndef OBJEKTI_H
#define OBJEKTI_H

#include <stdbool.h>

#define MAX_TELA 300

typedef struct {
    float x;
    float z;
}Metak;

typedef enum {
	TETRAEDAR  = 1,
	HEKSAEDAR  = 2,
	OKTAEDAR   = 3,
	DODEKAEDAR = 4,
	IKOSAEDAR  = 5,
    TORUS      = 6,
    SFERA      = 7,
    KONUS      = 8,
    PLANETA    = 9
}tip_tela;

typedef struct {
    int x;
    float y;
    bool is_platonic;
    //int polozaj_x_osa; 
    tip_tela tip;
}Telo;

void iscrtaj_ose(float len); 
void iscrtaj_metak();
Telo napravi_telo();
void inicijalizuj_tela();
void azuriraj_tela();
void nacrtaj_tela();
void nacrtaj_kosmos();
void initialise();
void ukloni_telo(char tip_tela);
void ispisi_tekst(char * tekst, int x, int y, int sirina_ekrana, int duzina_ekrana);

#endif

