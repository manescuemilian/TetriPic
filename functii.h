#ifndef FUNCTII
#define FUNCTII

#include "bmp_header.h"

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red; 
} pixel;

typedef struct {
	char piesa;
	int unghi;
	int coloana;
} comanda;

//initializeaza valorile din cele doua structuri
void initializare (file_h *F, info_h *I, int latime, int inaltime,
				int *extra_octeti);

//colorarea unei singure unitati
void unit (int inaltime, pixel px, int linie, 
		int coloana, pixel **M);

//functie de formare a matricii pentru task1
void formare_matrice (int inaltime, int latime, char piesa, pixel **M);

//functie de creare a imaginii
void creeaza_imagine (char *nume, file_h *F, info_h *I,
					int inaltime, int latime, char piesa);

//rotirea matricei de pixeli
pixel **rotire_matrice (int *latime, int *inaltime, pixel **M);

//functie de calculare a padding-ului
int padding (int latime);

//rotirea pieselor
void rotire_piesa (char *nume, char *n_imagine, int unghi,
				file_h *F, info_h *I);

//verifica daca doi pixeli sunt egali
int este_egal (pixel p1, pixel p2);

//verifica daca o linie contine culorile alb sau negru
int este_completa (pixel **M, int latime, int linie);

//mutarea liniilor dupa stergerea unei linii complete
void mutare_linii(pixel **M, int linie, int latime, int inaltime);

//sterge liniile complete din matrice
void sterge_linii_complete(pixel **M, int inaltime, int latime);

//verfica daca sub un patrat se afla pixeli colorati
int coliziune_patrat (pixel **M, int inaltime, 
					int linie, int coloana);

//verificare coliziune pentru fiecare patrat din piesa
//pentru piesele I, O, S, Z
int v_coliziune1 (pixel **M,int inaltime, int linie, 
						int coloana, char piesa, int unghi);

//pentru piesele L, J, T
int v_coliziune2 (pixel **M,int inaltime, int linie, 
						int coloana, char piesa, int unghi);

//verificare daca se iese din chenarul negru
int afara_tabla (int linie);

//functiile de desenare pentru fiecare piesa in parte
int desenare_albastru (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_galben (int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_rosu (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_verde (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_portocaliu1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_portocaliu2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_roz1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_roz2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_mov1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

int desenare_mov2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M);

//desenarea pieselor pe tabla de joc
int desenare_piesa (char piesa, int unghi, int coloana, 
					int inaltime, int latime, pixel **M);

void creare_tabla (pixel **M, int H, int L);

//formarea tablei de joc (task 3)
void desenare_tabla3 (char *nume);

//formarea tablei de joc (task 4)
void desenare_tabla4 (char *com, char *input);

//verifica ce culoare are un anumit patrat
int culoare_patrat (int lin, int col, pixel **M);

//functiile pentru verificarea pieselor din imaginea sursa
int albastru (int linie, int coloana, FILE *fisier, pixel **M);

int galben (int linie, int coloana, FILE *fisier, pixel **M);

int rosu (int linie, int coloana, FILE *fisier, pixel **M);

int verde (int linie, int coloana, FILE *fisier, pixel **M);

int portocaliu (int linie, int coloana, FILE *fisier, pixel **M);

int roz (int linie, int coloana, FILE *fisier, pixel **M);

int mov (int linie, int coloana, FILE *fisier, pixel **M);

//functia pentru task-ul bonus
void bonus (char *img, char *piesa);

#endif