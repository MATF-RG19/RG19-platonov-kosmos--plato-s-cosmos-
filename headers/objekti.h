#ifndef OBJEKTI_H
#define OBJEKTI_H

#include <stdbool.h>

#define MAX_TELA 500

typedef struct {
    float x;
    float z;
}Metak;

typedef enum {
	TETRAEDAR  = 1,
	HEKSAEDAR  = 2,
	OKTAEDAR   = 3,
	DODEKAEDAR = 4,
	IKOSAEDAR  = 5
}tip_tela;

typedef struct {
    float x;
    float z;
    bool is_pogodjena;
    float pogodjena_parametar;
    tip_tela tip;
}Plat_telo;

void iscrtaj_ose(); 
Plat_telo napravi_prepreke();
void nacrtaj_plat_tela();
void nacrtaj_kosmos();
void ispisi_tekst(char * tekst, int x, int y, int sirina_ekrana, int duzina_ekrana);
void initialise();

#endif

