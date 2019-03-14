#include <stdio.h>
#include "bmp_header.h"
#include "functii.h"

//Manescu Emilian Claudiu, 313 CB

int main(int argc, char *argv[]) {
	file_h F, F1;
	info_h I, I1;

	if (argc != 2 && argc != 1) {
		printf("Comanda gresita!\n");
	}

	if (argv[1][0] == '1') {
		creeaza_imagine ("piesa_I.bmp", &F, &I, 60, 30, 'I');
		creeaza_imagine ("piesa_O.bmp", &F, &I, 40, 40, 'O');
		creeaza_imagine ("piesa_S.bmp", &F, &I, 40, 50, 'S');
		creeaza_imagine ("piesa_Z.bmp", &F, &I, 40, 50, 'Z');
		creeaza_imagine ("piesa_L.bmp", &F, &I, 50, 40, 'L');
		creeaza_imagine ("piesa_J.bmp", &F, &I, 50, 40, 'J');
		creeaza_imagine ("piesa_T.bmp", &F, &I, 40, 50, 'T');

	} else if (argv[1][0] == '2') {
		//formeaza imaginile ce vor fi rotite
		creeaza_imagine ("piesa_I.bmp", &F, &I, 60, 30, 'I');
		creeaza_imagine ("piesa_O.bmp", &F, &I, 40, 40, 'O');
		creeaza_imagine ("piesa_S.bmp", &F, &I, 40, 50, 'S');
		creeaza_imagine ("piesa_Z.bmp", &F, &I, 40, 50, 'Z');
		creeaza_imagine ("piesa_L.bmp", &F, &I, 50, 40, 'L');
		creeaza_imagine ("piesa_J.bmp", &F, &I, 50, 40, 'J');
		creeaza_imagine ("piesa_T.bmp", &F, &I, 40, 50, 'T');

		//rotirea imaginilor
		//pentru I
		rotire_piesa ("piesa_I_90.bmp", "piesa_I.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_I_180.bmp", "piesa_I.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_I_270.bmp", "piesa_I.bmp", 270, &F1, &I1);

		//pentru O
		rotire_piesa ("piesa_O_90.bmp", "piesa_O.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_O_180.bmp", "piesa_O.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_O_270.bmp", "piesa_O.bmp", 270, &F1, &I1);

		//pentru S
		rotire_piesa ("piesa_S_90.bmp", "piesa_S.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_S_180.bmp", "piesa_S.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_S_270.bmp", "piesa_S.bmp", 270, &F1, &I1);

		//pentru Z
		rotire_piesa ("piesa_Z_90.bmp", "piesa_Z.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_Z_180.bmp", "piesa_Z.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_Z_270.bmp", "piesa_Z.bmp", 270, &F1, &I1);

		//pentru L
		rotire_piesa ("piesa_L_90.bmp", "piesa_L.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_L_180.bmp", "piesa_L.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_L_270.bmp", "piesa_L.bmp", 270, &F1, &I1);

		//pentru J
		rotire_piesa ("piesa_J_90.bmp", "piesa_J.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_J_180.bmp", "piesa_J.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_J_270.bmp", "piesa_J.bmp", 270, &F1, &I1);

		//pentru T
		rotire_piesa ("piesa_T_90.bmp", "piesa_T.bmp", 90, &F1, &I1);
		rotire_piesa ("piesa_T_180.bmp", "piesa_T.bmp", 180, &F1, &I1);
		rotire_piesa ("piesa_T_270.bmp", "piesa_T.bmp", 270, &F1, &I1);
	} else if (argv[1][0] == '3') {
		desenare_tabla3 ("cerinta3.in");

	} else if (argv[1][0] == '4') {
		desenare_tabla4 ("cerinta4.in", "cerinta4.bmp");

	} else if (argv[1][0] == '5') {
		bonus ("bonus.bmp", "bonus.out");
	}

	return 0;
}