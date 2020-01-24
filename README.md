# Platonov kosmos
### :milky_way: RG19-platonov-kosmos :milky_way:

![c](https://img.shields.io/badge/language-c-orange)
![opengl](https://img.shields.io/badge/lib-opengl-green.svg)
![glut](https://img.shields.io/badge/lib-glut-green.svg)

![alt text](platonic.gif?raw=true "Platonova tela")

## :pencil2: Opis igre
Igrač se nalazi u zamišljenom prostoru (*kosmosu*) među raznim telima koja padaju. 
Kada igrac uoči neko od pet **Platonovih tela**, uklanja ga pritiskom na odgovarajući taster (*t-tetraedar, h-heksaedar, o-oktaedar, d-dodekaedar, i-ikosaedar*). Ukoliko ne stigne, igrač gubi jedan od 20 života za svako Platonovo telo koje je prošlo. 

- Igrač pobeđuje ako za 60 sekundi stigne da ukloni 30 tela, a gubi ukoliko ostane bez svih života ili ukoliko istekne vreme.

- Pored Platonovih tela, u kosmosu se nalaze i druga tela kao i planete na koje igrač ne treba da reaguje.   

- Padanje tela se može dodatno ubrzati pritiskom na strelicu nadole (:arrow_down:).

## :wrench: Instalacija

**Instalacija biblioteke OpenGL:**
- `sudo apt-get install mesa-utils` 

**Instalacija igrice:**
- `git clone` https://github.com/MATF-RG19/RG19-platonov-kosmos--plato-s-cosmos-
- `cd RG19-platonov-kosmos--plato-s-cosmos-/`
- `make`
- `make run`

## :video_game: Komande 

| Komanda | Opis |
| --- | --- |
| <kbd> Enter </kbd> | Startuje igru   |
| <kbd> t </kbd> ili <kbd> T </kbd>| Uklanja tetraedar 	   		   |
| <kbd> h </kbd> ili <kbd> H </kbd>| Uklanja heksaedar 	   		   |
| <kbd> o </kbd> ili <kbd> O </kbd>| Uklanja oktaedar  	   		   |
| <kbd> d </kbd> ili <kbd> D </kbd>| Uklanja dodekaedar	   		   |
| <kbd> i </kbd> ili <kbd> I </kbd>| Uklanja ikosaedar 	   		   |
| <kbd> Esc </kbd>				   | Prekida igru - zatvara prozor |

## :mortar_board: Autor
Miloš Krsmanovic, 263/2015