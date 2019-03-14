#include <stdio.h>
#include <stdlib.h>
#include "bmp_header.h"
#include "functii.h"

//Manescu Emilian Claudiu, 313 CB

void initializare (file_h *F, info_h *I, int latime, int inaltime,
				 int *extra_octeti) {
	int offset, l_linie, dim;

	//initializarile prestabilite
	F -> fileMarker1 = 'B';
	F -> fileMarker2 = 'M';
	F -> unused1 = 0;
	F -> unused2 = 0;
	F -> imageDataOffset = 54;

	I -> biSize = 40;
	I -> planes = 1;
	I -> bitPix = 24;
	I -> biCompression = 0;
	I -> biXPelsPerMeter = 0;
	I -> biYPelsPerMeter = 0;
	I -> biClrUsed = 0;
	I -> biClrImportant = 0;

	offset = F -> imageDataOffset;

	//calcularea padding-ului pentru fisier
	*extra_octeti = 4 - ((latime * 3) % 4);

	if (*extra_octeti == 4) {
		*extra_octeti = 0;
	}

	l_linie = (latime * 3) + *extra_octeti;
	dim = l_linie * inaltime;

	//initializarile care depind de imaginea creata
	I -> width = latime;
	I -> height = inaltime;
	I -> biSizeImage = dim;

	F -> bfSize = offset + dim;
}

void unit (int inaltime, pixel px, int linie, 
		int coloana, pixel **M) {

	int i, j, h;
	int coloana_px = 10 * coloana;
	int linie_px = 10 * linie;
	h = inaltime - 1 - linie_px;

	for (i = h; i > h - 10; i--) {
		for (j = coloana_px; j < coloana_px + 10; j++) {
			M[i][j] = px;
		}
	}
}

void formare_matrice (int inaltime, int latime, char piesa, pixel **M) {
	//culorile
	pixel px_albastru = {255, 0, 0}, px_galben = {0, 255, 255};
	pixel px_rosu = {0, 0, 255}, px_verde = {0, 255, 0};
	pixel px_mov = {255, 0, 130}, px_roz = {255, 0, 255};
	pixel px_portocaliu = {0, 140, 255};
	int i, j;
	for (i = inaltime - 1; i >= 0; i--) {
		for (j = 0; j < latime; j++) {
			M[i][j].blue = 255; M[i][j].green = 255; M[i][j].red = 255;
			switch (piesa) {
				case 'I': 
					unit (inaltime, px_albastru, 1, 1, M);
					unit (inaltime, px_albastru, 2, 1, M);
					unit (inaltime, px_albastru, 3, 1, M);
					unit (inaltime, px_albastru, 4, 1, M);
					break;
				case 'O':
					unit (inaltime, px_galben, 1, 1, M);
					unit (inaltime, px_galben, 2, 1, M);
					unit (inaltime, px_galben, 1, 2, M);
					unit (inaltime, px_galben, 2, 2, M);
					break;
				case 'S':
					unit (inaltime, px_rosu, 1, 2, M);
					unit (inaltime, px_rosu, 1, 3, M);
					unit (inaltime, px_rosu, 2, 1, M);
					unit (inaltime, px_rosu, 2, 2, M);
					break;
				case 'Z':
					unit (inaltime, px_verde, 1, 1, M);
					unit (inaltime, px_verde, 1, 2, M);
					unit (inaltime, px_verde, 2, 2, M);
					unit (inaltime, px_verde, 2, 3, M);
					break;
				case 'L':
					unit (inaltime, px_portocaliu, 1, 1, M);
					unit (inaltime, px_portocaliu, 2, 1, M);
					unit (inaltime, px_portocaliu, 3, 1, M);
					unit (inaltime, px_portocaliu, 3, 2, M);
					break;
				case 'J':
					unit (inaltime, px_roz, 1, 2, M);
					unit (inaltime, px_roz, 2, 2, M);
					unit (inaltime, px_roz, 3, 2, M);
					unit (inaltime, px_roz, 3, 1, M);
					break;
				case 'T':
					unit (inaltime, px_mov, 1, 1, M);
					unit (inaltime, px_mov, 1, 2, M);
					unit (inaltime, px_mov, 1, 3, M);
					unit (inaltime, px_mov, 2, 2, M);
					break;
			}
		}
	}
}

//deseneaza patru patrate pentru a forma piesa
void creeaza_imagine (char *nume, file_h *F, info_h *I,
					  int inaltime, int latime, char piesa) {
	int i, j, k, extra_octeti = 0;
	initializare (F, I, latime, inaltime, &extra_octeti);
	pixel **M;
	M = malloc (inaltime * sizeof (pixel *));
	if (M == NULL) {
		exit (0);
	}
	for (i = 0; i < inaltime; i++) {
		M[i] = calloc (latime, sizeof (pixel));
		if (M[i] == NULL) {
			exit(0);
		}
	}
	FILE *imagine;
	imagine = fopen (nume, "wb");
	fwrite (F, sizeof (file_h), 1, imagine);
	fwrite (I, sizeof(info_h), 1, imagine);

	formare_matrice (inaltime, latime, piesa, M);

	for (i = 0; i < inaltime; i++) {
		for (j = 0; j < latime; j++) {
			fwrite (&M[i][j], sizeof (pixel), 1, imagine);
		}
		if (extra_octeti) {
			for (k = 1; k <= extra_octeti; k++) {
				fprintf (imagine, "%c", 0);
			}
		}
	}

	for (i = 0; i < inaltime; i++) {
		free (M[i]);
	}
	free (M);
	fclose (imagine);
}

pixel **rotire_matrice (int *latime, int *inaltime, pixel **M) {
	//cream matricea de dimensiune inversa
	pixel **R;
	int i, j, x = 0, aux;
	R = malloc (*latime * sizeof (pixel *));
	if (R == NULL) {
		exit (0);
	}
	for (i = 0; i < *latime; i++) {
		R[i] = malloc (*inaltime * sizeof (pixel));
		if (R[i] == NULL) {
			exit(0);
		}
	}

	//salvam in matrice elementele, dupa rotire
	for (i = 0; i < *latime; i++) {
		for (j = *inaltime - 1; j >= 0; j--) {
			R[i][x++] = M[j][i];
		}
		x = 0;
	}

	//eliberam matricea, deoarece va deveni egala cu R dupa rotire
	for (i = 0; i < *inaltime; i++) {
		free (M[i]);
	}
	free (M);

	//interschimbarea latimii si inaltimii
	aux = *latime;
	*latime = *inaltime;
	*inaltime = aux;
	return R;
}

int padding (int latime) {
	int padding;
	padding = 4 - ((latime * 3) % 4);
	if (padding == 4) {
		padding = 0;
	}
	return padding;
}

void rotire_piesa (char *nume, char *n_imagine, int unghi,
				file_h *F, info_h *I) {
	int i, j, k, extra_octeti = 0, extra_octeti_m = 0, latime, 
		inaltime, latime_m, inaltime_m;
	unsigned int offset, dim, aux;
	FILE *imagine, *piesa;
	piesa = fopen (n_imagine, "rb");
	fread (F, sizeof (file_h), 1, piesa);
	fread (I, sizeof (info_h), 1, piesa);
	offset = F -> imageDataOffset;
	latime_m = I -> width, inaltime_m = I -> height;
	pixel **M = malloc (inaltime_m * sizeof (pixel *));
	if (M == NULL) {
		exit (0);
	}
	for (i = 0; i < inaltime_m; i++) {
		M[i] = malloc (latime_m * sizeof (pixel));
		if (M[i] == NULL) {
			exit(0);
		}
	}

	//padding pentru fisierul deschis
	extra_octeti_m = padding (latime_m);

	//actualizarea dimensiunilor imaginii in functie de unghi
	if (unghi == 90 || unghi == 270) {
		aux = I -> width;
		I -> width = I -> height;
		I -> height = aux;
	}

	latime = I -> width, inaltime = I -> height;
	extra_octeti = padding (latime);

	dim = ((latime * 3) + extra_octeti) * inaltime;
	I -> biSizeImage = dim, F -> bfSize = offset + dim;
	imagine = fopen (nume, "wb");
	fwrite (F, sizeof (file_h), 1, imagine);
	fwrite (I, sizeof(info_h), 1, imagine);

	for (i = 0; i < inaltime_m; i++) {
		for (j = 0; j < latime_m; j++) {
			fread (&M[i][j], sizeof(pixel), 1, piesa);
		}
		fseek (piesa, extra_octeti_m, SEEK_CUR);
	}

	if (unghi == 90) {
		M = rotire_matrice (&latime_m, &inaltime_m, M);
		M = rotire_matrice (&latime_m, &inaltime_m, M);
		M = rotire_matrice (&latime_m, &inaltime_m, M);

	} else if (unghi == 180) {
		M = rotire_matrice (&latime_m, &inaltime_m, M);
		M = rotire_matrice (&latime_m, &inaltime_m, M);

	} else if (unghi == 270) {
		M = rotire_matrice (&latime_m, &inaltime_m, M);
	}

	for (i = 0; i < inaltime; i++) {
		for (j = 0; j < latime; j++) {
			fwrite (&M[i][j], sizeof (pixel), 1, imagine);
		}
		if (extra_octeti) {
			for (k = 1; k <= extra_octeti; k++) {
				fprintf (imagine, "%c", 0);
			}
		}
	}

	for (i = 0; i < inaltime; i++) {
		free (M[i]);
	}
	free (M);
	fclose (piesa); fclose (imagine);
}

int este_egal (pixel p1, pixel p2) {
	if (p1.blue != p2.blue ||
		p1.green != p2.green ||
		p1.red != p2.red) {
		return 0;
	}

	return 1;
}

int este_completa (pixel **M, int latime, int linie) {
	pixel px_negru = {0, 0, 0}, px_alb = {255, 255, 255};
	int i, j, lpx = linie * 10;

	//daca se gaseste macar un bit alb sau negru, linia nu e completa
	for (i = lpx; i < lpx + 10; i++) {
		for (j = 0; j < latime; j++) {
			if (este_egal (M[i][j], px_negru) 
				|| este_egal (M[i][j], px_alb)) {
				return 0;
			}
		}
	}
	return 1;
}

void mutare_linii(pixel **M, int linie, int latime, int inaltime) {
	int i, j;
	pixel px_negru = {0, 0, 0};
	for (i = linie; i < inaltime - 40; i++) {
		for (j = 0; j < latime; j++) {
			M[i][j] = M[i + 10][j];
		}
	}
	//prima linie de deasupra chenarului alb va deveni neagra
	for (i = inaltime - 50; i < inaltime - 40; i++) {
		for (j = 0; j < latime; j++) {
			M[i][j] = px_negru;
		}
	}
}

//parcurg matricea, verific daca e plina
//mut tot ce e deasupra liniei in jos cu o linie
void sterge_linii_complete(pixel **M, int inaltime, int latime) {
	int i;
	for (i = 0; i < inaltime / 10; i++)
		if (este_completa (M, latime, i)) {
			//toate liniile de sus coboara cu o linie
			mutare_linii (M, 10 * i, latime, inaltime);
			i = 0;
		}
}

int coliziune_patrat (pixel **M, int inaltime, 
					int linie, int coloana) {
	int i, j, h, l;
	pixel px_negru = {0, 0, 0}, px_alb = {255, 255, 255};
	h = inaltime - linie * 10 - 10;
	l = coloana * 10;
	if (h <= 0) {
		return 2;
	}

	for (i = h - 1; i >= h - 10; i--) {
		for (j = l; j < l + 10; j++) {
			//daca sub patrat se gasesc pixeli diferiti de alb sau negru
			if (!este_egal (M[i][j], px_alb) &&
				!este_egal (M[i][j], px_negru)) {
				return 1;
			}
		}
	}
	return 0;
}

int v_coliziune1 (pixel **M, int inaltime, int linie, 
						int coloana, char piesa, int unghi) {
	if (piesa == 'I') {
		if ((unghi == 90 || unghi == 270)) {
			if (coliziune_patrat (M, inaltime, linie, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 2) == 1) {
				return 1;
			}
		} else if (unghi == 0) {
			if (coliziune_patrat (M, inaltime, linie + 3, coloana) == 1) {
				return 1;
			}
		}
	} else if (piesa == 'O') {
		if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 || 
			coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1) {
				return 1;
		}
	} else if (piesa == 'S') {
		if (unghi == 90 || unghi == 270) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 2, coloana + 1) == 1) {
				return 1;
			}
		} else if (unghi == 0) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 2) == 1) {
				return 1;
			}
		}
	} else if (piesa == 'Z') {
		if (unghi == 90 || unghi == 270) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie + 2, coloana) == 1) {
				return 1;
			}
		} else if (unghi == 0) {
			if (coliziune_patrat (M, inaltime, linie, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 2) == 1) {
				return 1;
			}
		}
	}
	return 0;
}

int v_coliziune2 (pixel **M,int inaltime, int linie, 
						int coloana, char piesa, int unghi) {
	if (piesa == 'L') {
		if (unghi == 0) {
			if (coliziune_patrat (M, inaltime, linie + 2, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 2, coloana + 1) == 1) {
				return 1;
			}
		} else if (unghi == 90) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 2) == 1) {
				return 1;
			}
		} else if (unghi == 180) {
			if (coliziune_patrat (M, inaltime, linie, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 2, coloana + 1) == 1) {
				return 1;
			}
		} else if (unghi == 270) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 2) == 1) {
				return 1;
			}
		}
	} else if (piesa == 'J') {
		if (unghi == 0) {
			if (coliziune_patrat (M, inaltime, linie + 2, coloana) == 1||
				coliziune_patrat (M, inaltime, linie + 2, coloana + 1) == 1) {
				return 1;
			}
		} else if (unghi == 90) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 2) == 1) {
				return 1;
			}
		} else if (unghi == 180) {
			if (coliziune_patrat (M, inaltime, linie, coloana + 1) == 1||
				coliziune_patrat (M, inaltime, linie + 2, coloana) == 1) {
				return 1;
			}

		} else if (unghi == 270) {
			if (coliziune_patrat (M, inaltime, linie, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie, coloana + 1) == 1||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 2) == 1) {
				return 1;
			}
		}
	} else if (piesa == 'T') {
		if (unghi == 90) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1||
				coliziune_patrat (M, inaltime, linie + 2, coloana + 1) == 1) {
				return 1;
			}
		} else if (unghi == 180) {
			if (coliziune_patrat (M, inaltime, linie + 1, coloana) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1 ||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 2) == 1) {
				return 1;
			}
		} else if (unghi == 270) {
			if (coliziune_patrat (M, inaltime, linie + 2, coloana) == 1||
				coliziune_patrat (M, inaltime, linie + 1, coloana + 1) == 1) {
				return 1;
			}
		}
	}
	return 0;
}

int afara_tabla (int linie) {
	if (linie <= 4 && linie >= 0) {
		return 1;
	}

	return 0;
}

int desenare_albastru (int unghi, int inaltime, int latime, 
					int coloana, pixel **M) {
	int lin = 0;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_albastru = {255, 0, 0};
	lin = 1;
	if (unghi == 90 || unghi == 270) {
		while (lin != (inaltime / 10)) {
			//daca se afla in chenarul alb
			//stergerea piesei precendete, dupa coborarea cu o linie
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				unit (inaltime, px_alb, lin - 1, coloana + 2, M);
				unit (inaltime, px_alb, lin - 1, coloana + 3, M);
			//daca se afla in chenarul negru	
			} else {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				unit (inaltime, px_negru, lin - 1, coloana + 2, M);
				unit (inaltime, px_negru, lin - 1, coloana + 3, M);	
			}
			//desenarea piesei propriu-zise
			unit (inaltime, px_albastru, lin, coloana, M);
			unit (inaltime, px_albastru, lin, coloana + 1, M);
			unit (inaltime, px_albastru, lin, coloana + 2, M);
			unit (inaltime, px_albastru, lin, coloana + 3, M);

			//verificarea coliziunii
			if (v_coliziune1 (M, inaltime, lin, coloana, 'I', 90)) {
				break;
			}
			lin++;
		}

	} else if (unghi == 0 || unghi == 180) {
		while ((lin + 3) != inaltime / 10) {
			if (lin >= 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin, coloana, M);
				unit (inaltime, px_alb, lin + 2, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana, M);	
			} else {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin, coloana, M);
				unit (inaltime, px_negru, lin + 1, coloana, M);
				unit (inaltime, px_negru, lin + 2, coloana, M);	
			}

			unit (inaltime, px_albastru, lin, coloana, M);
			unit (inaltime, px_albastru, lin + 1, coloana, M);
			unit (inaltime, px_albastru, lin + 2, coloana, M);
			unit (inaltime, px_albastru, lin + 3, coloana, M);
			if (v_coliziune1 (M, inaltime, lin, coloana, 'I', 0)) {
				break;
			}	
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_galben (int inaltime, int latime,  
					int coloana, pixel **M) {
	int lin = 0;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_galben = {0, 255, 255};

	while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}

			unit (inaltime, px_galben, lin, coloana, M);
			unit (inaltime, px_galben, lin, coloana + 1, M);
			unit (inaltime, px_galben, lin + 1, coloana, M);
			unit (inaltime, px_galben, lin + 1, coloana + 1, M);
			if (v_coliziune1 (M, inaltime, lin, coloana, 'O', 0)) {
				break;
			}
			lin++;
		}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_rosu (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_rosu = {0, 0, 255};

	int lin = 0;
	if (unghi == 90 || unghi == 270) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana + 1, M);
				} else {
					unit (inaltime, px_alb, lin, coloana + 1, M);
				}

			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin , coloana + 1, M);
			}

			unit (inaltime, px_rosu, lin, coloana, M);
			unit (inaltime, px_rosu, lin + 1, coloana, M);
			unit (inaltime, px_rosu, lin + 1, coloana + 1, M);
			unit (inaltime, px_rosu, lin + 2, coloana + 1, M);
			if (v_coliziune1 (M, inaltime, lin, coloana, 'S', 90)) {
				//daca piesa iese afara din tabla
				if (afara_tabla (lin)) {
					return 1;
				}
				break;	
			}
			lin++;
		}	
	} else if (unghi == 0) {
		while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				unit (inaltime, px_alb, lin - 1, coloana + 2, M);
				unit (inaltime, px_alb, lin, coloana, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana, M);
				} else {
					unit (inaltime, px_alb, lin, coloana, M);
				}

			} else if (lin > 4) {
				unit (inaltime, px_negru, lin, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				unit (inaltime, px_negru, lin - 1, coloana + 2, M);
			}

			unit (inaltime, px_rosu, lin + 1, coloana, M);
			unit (inaltime, px_rosu, lin + 1, coloana + 1, M);
			unit (inaltime, px_rosu, lin, coloana + 2, M);
			unit (inaltime, px_rosu, lin, coloana + 1, M);
			if (v_coliziune1 (M, inaltime, lin, coloana, 'S', unghi)) {
				break;
			}
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_verde (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	int lin = 0;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_verde = {0, 255, 0};
	if (unghi == 90 || unghi == 270) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana, M);
				} else {
					unit (inaltime, px_alb, lin, coloana, M);
				}
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}

			unit (inaltime, px_verde, lin, coloana + 1, M);
			unit (inaltime, px_verde, lin + 1, coloana, M);
			unit (inaltime, px_verde, lin + 1, coloana + 1, M);
			unit (inaltime, px_verde, lin + 2, coloana, M);
			if (v_coliziune1 (M, inaltime, lin, coloana, 'Z', 90)) {
				break;
			}
			lin++;
		}		
	} else if (unghi == 0) {
		while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana + 2, M);
				} else {
					unit (inaltime, px_alb, lin , coloana + 2, M);
				}
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				unit (inaltime, px_negru, lin, coloana + 2, M);
			}

			unit (inaltime, px_verde, lin, coloana, M);
			unit (inaltime, px_verde, lin, coloana + 1, M);
			unit (inaltime, px_verde, lin + 1, coloana + 1, M);
			unit (inaltime, px_verde, lin + 1, coloana + 2, M);

			if (v_coliziune1 (M, inaltime, lin, coloana, 'Z', 0)) {
				if (afara_tabla (lin)) {
					return 1;
				}
				break;
			}
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_portocaliu1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	int lin = 0;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_portocaliu = {0, 140, 255};

	if (unghi == 90) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				unit (inaltime, px_alb, lin - 1, coloana + 2, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				unit (inaltime, px_negru, lin - 1, coloana + 2, M);
			}

			unit (inaltime, px_portocaliu, lin, coloana, M);
			unit (inaltime, px_portocaliu, lin, coloana + 1, M);
			unit (inaltime, px_portocaliu, lin, coloana + 2, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'L', unghi)) {
				break;
			}
			lin++;
		}

	} else if (unghi == 180) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}

			unit (inaltime, px_portocaliu, lin, coloana, M);
			unit (inaltime, px_portocaliu, lin, coloana + 1, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana + 1, M);
			unit (inaltime, px_portocaliu, lin + 2, coloana + 1, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'L', unghi)) {
				break;
			}
			lin++;
		}

	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_portocaliu2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	int lin = 0;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_portocaliu = {0, 140, 255};
	if (unghi == 270) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana, M);
					unit (inaltime, px_negru, lin, coloana + 1, M);
				} else {
					unit (inaltime, px_alb, lin, coloana, M);
					unit (inaltime, px_alb, lin, coloana + 1, M);
				}
				unit (inaltime, px_alb, lin - 1, coloana + 2, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin, coloana, M);
				unit (inaltime, px_negru, lin, coloana + 1, M);
				unit (inaltime, px_negru, lin - 1, coloana + 2, M);
			}

			unit (inaltime, px_portocaliu, lin, coloana + 2, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana + 1, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana + 2, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'L', 270)) {
				break;
			}
			lin++;
		}
	} else if (unghi == 0) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin, coloana, M);
				if (lin <= 2) {
					unit (inaltime, px_alb, lin + 1, coloana, M);
					unit (inaltime, px_alb, lin + 1, coloana + 1, M);
				} else {
					unit (inaltime, px_negru, lin + 1, coloana, M);
					unit (inaltime, px_negru, lin + 1,coloana + 1, M);
				}
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin , coloana, M);
				unit (inaltime, px_negru, lin + 1, coloana, M);
				unit (inaltime, px_negru, lin + 1, coloana + 1, M);
			}
			unit (inaltime, px_portocaliu, lin, coloana, M);
			unit (inaltime, px_portocaliu, lin + 1, coloana, M);
			unit (inaltime, px_portocaliu, lin + 2, coloana, M);
			unit (inaltime, px_portocaliu, lin + 2, coloana + 1, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'L', 0)) {
				break;
			}
			lin++;
		}		
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_roz1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_roz = {255, 0, 255};
	int lin = 0;
	if (unghi == 90) {
		while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana + 1, M);
					unit (inaltime, px_negru, lin, coloana + 2, M);
				} else {
					unit (inaltime, px_alb, lin, coloana + 1, M);
					unit (inaltime, px_alb, lin, coloana + 2, M);
				}
				
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin, coloana + 1, M);
				unit (inaltime, px_negru, lin, coloana + 2, M);
			}

			unit (inaltime, px_roz, lin, coloana, M);
			unit (inaltime, px_roz, lin + 1, coloana, M);
			unit (inaltime, px_roz, lin + 1, coloana + 1, M);
			unit (inaltime, px_roz, lin + 1, coloana + 2, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'J', 90)) {
				break;
			}
			lin++;
		}
	} else if (unghi == 180) {
		while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}

			unit (inaltime, px_roz, lin, coloana, M);
			unit (inaltime, px_roz, lin, coloana + 1, M);
			unit (inaltime, px_roz, lin + 1, coloana, M);
			unit (inaltime, px_roz, lin + 2, coloana, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'J', 180)) {
				break;
			}
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_roz2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_roz = {255, 0, 255};
	int lin = 0;
	if (unghi == 270) {
		while ((lin + 1) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				unit (inaltime, px_alb, lin - 1, coloana + 2, M);
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				unit (inaltime, px_negru, lin - 1, coloana + 2, M);
			}

			unit (inaltime, px_roz, lin, coloana, M);
			unit (inaltime, px_roz, lin, coloana + 1, M);
			unit (inaltime, px_roz, lin, coloana + 2, M);
			unit (inaltime, px_roz, lin + 1, coloana + 2, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'J', 270)) {
				break;
			}
			lin++;
		}
	} else if (unghi == 0) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				unit (inaltime, px_alb, lin + 1, coloana, M);
				if (lin == 4 || lin == 3) {
					unit (inaltime, px_negru, lin + 1, coloana, M);
				} else {
					unit (inaltime, px_alb, lin + 1, coloana, M);
				}
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin + 1, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}

			unit (inaltime, px_roz, lin, coloana + 1, M);
			unit (inaltime, px_roz, lin + 1, coloana + 1, M);
			unit (inaltime, px_roz, lin + 2, coloana + 1, M);
			unit (inaltime, px_roz, lin + 2, coloana, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'J', 0)) {
				if (afara_tabla (lin)) {
					return 1;
				}
				break;
			}
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_mov1 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_mov = {255, 0, 130};
	int lin = 0;
	if (unghi == 90) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana + 1, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana, M);
				} else {
					unit (inaltime, px_alb, lin, coloana, M);
				}
				
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin, coloana, M);
				unit (inaltime, px_negru, lin - 1, coloana + 1, M);
			}
			unit (inaltime, px_mov, lin, coloana + 1, M);
			unit (inaltime, px_mov, lin + 1, coloana, M);
			unit (inaltime, px_mov, lin + 1, coloana + 1, M);
			unit (inaltime, px_mov, lin + 2, coloana + 1, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'T', 90)) {
				if (afara_tabla (lin)) {
					return 1;
				}
				break;
			}
			lin++;
			}
		} else if (unghi == 180) {
			while ((lin + 1) != inaltime / 10) {
				if (lin > 0 && lin <= 4) {
					unit (inaltime, px_alb, lin - 1, coloana + 1, M);
					if (lin == 4) {
						unit (inaltime, px_negru, lin, coloana + 2, M);
						unit (inaltime, px_negru, lin, coloana, M);
					} else {
						unit (inaltime, px_alb, lin, coloana + 2, M);
						unit (inaltime, px_alb, lin, coloana, M);
					}
					
				} else if (lin > 4) {
					unit (inaltime, px_negru, lin, coloana, M);
					unit (inaltime, px_negru, lin , coloana + 2, M);
					unit (inaltime, px_negru, lin - 1, coloana + 1, M);
				}

				unit (inaltime, px_mov, lin, coloana + 1, M);
				unit (inaltime, px_mov, lin + 1, coloana, M);
				unit (inaltime, px_mov, lin + 1, coloana + 1, M);
				unit (inaltime, px_mov, lin + 1, coloana + 2, M);
				if (v_coliziune2 (M, inaltime, lin, coloana, 'T', 180)) {
					if (afara_tabla (lin)) {
						return 1;
					}
					break;				
				}
				lin++;
			}

	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_mov2 (int unghi, int inaltime, int latime,  
					int coloana, pixel **M) {
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	pixel px_mov = {255, 0, 130};
	int lin = 0;
	if (unghi == 270) {
		while ((lin + 2) != inaltime / 10) {
			if (lin > 0 && lin <= 4) {
				unit (inaltime, px_alb, lin - 1, coloana, M);
				unit (inaltime, px_alb, lin , coloana, M);
				unit (inaltime, px_alb, lin, coloana + 1, M);
				if (lin == 4) {
					unit (inaltime, px_negru, lin, coloana + 1, M);
				} else {
					unit (inaltime, px_alb, lin, coloana + 1, M);
				}
				
			} else if (lin > 4) {
				unit (inaltime, px_negru, lin - 1, coloana, M);
				unit (inaltime, px_negru, lin , coloana, M);
				unit (inaltime, px_negru, lin, coloana + 1, M);
				unit (inaltime, px_negru, lin + 1, coloana, M);
			}

			unit (inaltime, px_mov, lin, coloana, M);
			unit (inaltime, px_mov, lin + 1, coloana, M);
			unit (inaltime, px_mov, lin + 1, coloana + 1, M);
			unit (inaltime, px_mov, lin + 2, coloana, M);
			if (v_coliziune2 (M, inaltime, lin, coloana, 'T', 270)) {
				break;
			}
			lin++;
		}
	}
	sterge_linii_complete (M, inaltime, latime);
	return 0;
}

int desenare_piesa (char piesa, int unghi, int coloana, 
					int inaltime, int latime, pixel **M) {
	//verificarea piesei ce va fi mutate
	//pentru fiecare piesa, se verifica ce rotire are
	if (piesa == 'I') {
		if (desenare_albastru (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}

	} else if (piesa == 'O') {
		if (desenare_galben (inaltime, latime, coloana, M)) {
			return 1;
		}

	} else if (piesa == 'S') {
		if (desenare_rosu (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}

	} else if (piesa == 'Z') {
		if (desenare_verde (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}
	} else if (piesa == 'L') {
		if (desenare_portocaliu1 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}

		if (desenare_portocaliu2 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}
	} else if (piesa == 'J') {
		if (desenare_roz1 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}

		if (desenare_roz2 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}
	} else if (piesa == 'T') {
		if (desenare_mov1 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}

		if (desenare_mov2 (unghi, inaltime, latime, coloana, M)) {
			return 1;
		}
	}
	return 0;
}

void creare_tabla (pixel **M, int H, int L) {
	int i, j;
	pixel px_alb = {255, 255, 255}, px_negru = {0, 0, 0};
	for (i = H - 1; i >= 0; i--) {
		for (j = 0; j < L; j++) {
			if (i < H && i >= H - 40) {
				M[i][j] = px_alb;
			} else {
				M[i][j] = px_negru;
			}
		}
	}
}

void desenare_tabla3 (char *nume) {
	FILE *comenzi, *task3;
	comenzi = fopen (nume, "rt");
	int N, H, L, hpx, lpx, i, j, k, extra_octeti = 0;
	//vector de elemente de tip comanda
	comanda *C;
	file_h F; info_h I;

	//citirea valorilor
	fscanf (comenzi, "%d", &N);
	C = malloc (N * sizeof (comanda));
	fscanf (comenzi, "%d", &H);
	fscanf (comenzi, "%d", &L);

	//dimensiunile tablei de joc
	//adunarea cu 40 vine de la chenarul alb, care trebuie inclus in tabla
	hpx = 10 * H + 40, lpx = 10 * L;
	for (i = 0; i < N; i++) {
		fscanf (comenzi, "%s", &C[i].piesa);
		fscanf (comenzi, "%d", &C[i].unghi);
		fscanf (comenzi, "%d", &C[i].coloana);
	}

	//deschiderea imaginii si initializare file si info header
	task3 = fopen ("task3.bmp", "wb");
	initializare (&F, &I, lpx, hpx, &extra_octeti);

	pixel **M = malloc (hpx * sizeof (pixel *));
	if (M == NULL) {
		exit (0);
	}
	for (i = 0; i < hpx; i++) {
		M[i] = malloc (lpx * sizeof (pixel));
		if (M[i] == NULL) {
			exit(0);
		}
	}

	creare_tabla (M, hpx, lpx);

	//parcurgerea mutarilor
	for (i = 0; i < N; i++) {
		//daca se iese din tabla de joc, se ignora restul mutarilor
		if (desenare_piesa (C[i]. piesa, C[i].unghi, C[i].coloana, 
			hpx, lpx, M)) {
			i = N;
		};
	}
	free (C);

	//scrierea in fisier
	fwrite (&F, sizeof (file_h), 1, task3);
	fwrite (&I, sizeof(info_h), 1, task3);
 
	for (i = 0; i < hpx; i++) {
		for (j = 0; j < lpx; j++) {
			fwrite (&M[i][j], sizeof(pixel), 1, task3);
		} 

		if (extra_octeti) {
			for (k = 1; k <= extra_octeti; k++) {
				fprintf (task3, "%c", 0);
			}
		}
	}
	fclose (comenzi);
	fclose (task3);
	for (i = 0; i < hpx; i++) {
		free (M[i]);
	}
	free (M);
}

void desenare_tabla4 (char *com, char *input) {
	FILE *comenzi, *task4, *bmp;
	int N, H, L, i, j, k, extra_octeti = 0;
	//vector de elemente de tip comanda
	comanda *C;
	file_h F;
	info_h I;
	task4 = fopen ("task4.bmp", "wb");
	comenzi = fopen (com, "rt");
	bmp = fopen (input, "rb");
	pixel **M;

	//citirea numarului de mutari
	fscanf (comenzi, "%d", &N);
	C = malloc (N * sizeof (comanda));

	for (i = 0; i < N; i++) {
		//citim o linie din fisier
		fscanf (comenzi, "%s", &C[i].piesa);
		fscanf (comenzi, "%d", &C[i].unghi);
		fscanf (comenzi, "%d", &C[i].coloana);
	}

	//copierea file si info header din imaginea de input in imaginea finala
	fread (&F, sizeof (file_h), 1, bmp);
	fread (&I, sizeof (info_h), 1, bmp);
	fwrite (&F, sizeof (file_h), 1, task4);
	fwrite (&I, sizeof (info_h), 1, task4);

	H = I.height, L = I.width;
	M = malloc (H * sizeof (pixel *));
	if (M == NULL) {
		exit (0);
	}
	for (i = 0; i < H; i++) {
		M[i] = malloc (L * sizeof (pixel));
		if (M[i] == NULL) {
			exit(0);
		}
	}
	//calcularea padding
	extra_octeti = 4 - ((L * 3) % 4);
	if (extra_octeti == 4) {
		extra_octeti = 0;
	}

	//copierea bitmap-ului (cu ignorarea padding-ului)
	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fread (&M[i][j], sizeof (pixel), 1, bmp);
		}
		fseek (bmp, extra_octeti, SEEK_CUR);
	}

	for (i = 0; i < N; i++) {
		if (desenare_piesa (C[i]. piesa, C[i].unghi, C[i].coloana, 
			H, L, M)) {
			i = N;
		}
	}
	free (C);

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fwrite (&M[i][j], sizeof(pixel), 1, task4);
		} 
		if (extra_octeti) {
			for (k = 1; k <= extra_octeti; k++) {
				fprintf (task4, "%c", 0);
			}
		}
	}
	for (i = 0; i < H; i++) {
		free (M[i]);
	}
	free (M);
	fclose (comenzi);
	fclose (task4);
	fclose (bmp);
}

int culoare_patrat (int lin, int col, pixel **M) {
	int linie = 10 * lin, coloana = 10 * col;
	pixel px_albastru = {255, 0, 0}, px_galben = {0, 255, 255};
	pixel px_rosu = {0, 0, 255}, px_verde = {0, 255, 0};
	pixel px_portocaliu = {0, 140, 255}, px_roz = {255, 0, 255};
	pixel px_mov = {255, 0, 130};

	//verificarea culorii unui patrat
	//valorile returnate sunt diferite in functie de culoare
	if (linie < 0) {
		return -1;
	}
	if (este_egal (M[linie][coloana], px_albastru) == 1) {
		return 1;
	}  else if (este_egal (M[linie][coloana], px_galben) == 1) {
		return 2;
	} else if (este_egal (M[linie][coloana], px_rosu) == 1) {
		return 3;
	} else if (este_egal (M[linie][coloana], px_verde) == 1) {
		return 4;
	} else if (este_egal (M[linie][coloana], px_portocaliu) == 1) {
		return 5;
	} else if (este_egal (M[linie][coloana], px_roz) == 1) {
		return 6;
	} else if (este_egal (M[linie][coloana], px_mov) == 1) {
		return 7;
	}
	return 0;
}

int albastru (int linie, int coloana, FILE *fisier, pixel **M) {

	/* verificarea, in functie de patratul de inceput, 
	a culorii patratului de inceput */
	if (culoare_patrat (linie + 1, coloana, M) == 1 &&
		culoare_patrat (linie + 2, coloana, M) == 1 &&
		culoare_patrat (linie + 3, coloana, M) == 1 &&
		culoare_patrat (linie - 1, coloana, M) != 1) {
		fprintf(fisier, "I 0 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie, coloana + 1, M) == 1 &&
		culoare_patrat (linie, coloana + 2, M) == 1 &&
		culoare_patrat (linie, coloana + 3, M) == 1) {
		fprintf(fisier, "I 90 %d\n", coloana);
		return 1;
	}

	return 0;
}

int galben (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie, coloana + 1, M) == 2 &&
		culoare_patrat (linie + 1, coloana, M) == 2 &&
		culoare_patrat (linie + 1, coloana + 1, M) == 2) {
			fprintf(fisier, "O 0 %d\n", coloana);
			return 1;
		} 

	return 0;
}

int rosu (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie, coloana + 1, M) == 3 &&
		culoare_patrat (linie + 1, coloana + 1, M) == 3 &&
		culoare_patrat (linie + 1, coloana + 2, M) == 3) {
		fprintf (fisier, "S 0 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie - 1, coloana, M) == 3 &&
		culoare_patrat (linie - 1, coloana + 1, M) == 3 &&
		culoare_patrat (linie - 2, coloana + 1, M) == 3) {
		fprintf (fisier, "S 90 %d\n", coloana);
		return 1;
	}

	return 0;
}

int verde (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie + 1, coloana, M) == 4 &&
		culoare_patrat (linie + 1, coloana + 1, M) == 4 &&
		culoare_patrat (linie + 2, coloana + 1, M) == 4) {
		fprintf(fisier, "Z 90 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie, coloana + 1, M) == 4 &&
		culoare_patrat (linie - 1, coloana + 1, M) == 4 &&
		culoare_patrat (linie - 1, coloana + 2, M) == 4) {
		fprintf(fisier, "Z 0 %d\n", coloana);
		return 1;
	}

	return 0;
}

int portocaliu (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie - 1, coloana, M) == 5 &&
		culoare_patrat (linie - 2, coloana, M) == 5 &&
		culoare_patrat (linie - 2, coloana + 1, M) == 5 &&
		culoare_patrat (linie, coloana, M) == 5) {
		fprintf(fisier, "L 0 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie, coloana + 1, M) == 5 &&
		culoare_patrat (linie, coloana + 2, M) == 5 &&
		culoare_patrat (linie + 1, coloana + 2, M) == 5) {
		if (culoare_patrat (linie - 1, coloana + 1, M) == 5 &&
			culoare_patrat (linie, coloana + 2, M) == 5 &&
			culoare_patrat (linie, coloana + 3, M) == 5) {
		} else {
			fprintf(fisier, "L 270 %d\n", coloana);
			return 1;
		}
	} else if (culoare_patrat (linie - 1, coloana, M) == 5 &&
		culoare_patrat (linie, coloana + 1, M) == 5 &&
		culoare_patrat (linie, coloana + 2, M) == 5) {
		fprintf(fisier , "L 90 %d\n", coloana);
		return 1;
	}

	return 0;
}

int roz (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie + 1, coloana, M) == 6 &&
		culoare_patrat (linie + 2, coloana, M) == 6 &&
		culoare_patrat (linie + 2, coloana + 1, M) == 6) {
		fprintf(fisier, "J 180 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie, coloana + 1, M) == 6 &&
		culoare_patrat (linie, coloana + 2, M) == 6 &&
		culoare_patrat (linie - 1, coloana + 2, M) == 6) {
		fprintf(fisier, "J 270 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie - 1, coloana, M) == 6 &&
		culoare_patrat (linie - 1, coloana + 1, M) == 6 &&
		culoare_patrat (linie - 1, coloana + 2, M) == 6) {
		fprintf(fisier, "J 90 %d\n", coloana);
		return 1;
	}

	return 0;
}

int mov (int linie, int coloana, FILE *fisier, pixel **M) {

	if (culoare_patrat (linie, coloana + 1, M) == 7 &&
		culoare_patrat (linie - 1, coloana + 1, M) == 7 &&
		culoare_patrat (linie + 1, coloana + 1, M) == 7) {
		fprintf(fisier, "T 90 %d\n", coloana);
		return 1;
	} else if (culoare_patrat (linie - 1, coloana, M) == 7 &&
		culoare_patrat (linie - 1, coloana + 1, M) == 7 &&
		culoare_patrat (linie - 2, coloana, M) == 7) {
		fprintf(fisier, "T 270 %d\n", coloana);
		return 1;
	}
	return 0;
}

void bonus (char *img, char *output) {
	FILE *imagine, *piese;
	file_h F;
	info_h I;
	int i, j, numar = 0, extra_octeti, L, H, lpx, hpx;
	imagine = fopen (img, "rb");
	fread (&F, sizeof (file_h), 1, imagine);
	fread (&I, sizeof (info_h), 1, imagine);
	pixel **M;

	lpx = I.width, hpx = I.height;
	L = lpx / 10, H = hpx / 10;
	
	extra_octeti = padding (lpx);

	M = malloc (hpx * sizeof (pixel *));
	if (M == NULL) {
		exit (0);
	}
	for (i = 0; i < hpx; i++) {
		M[i] = malloc (lpx * sizeof (pixel));
		if (M[i] == NULL) {
			exit(0);
		}
	}

	//salvam pixelii intr-o matrice
	for (i = 0; i < hpx; i++) {
		for (j = 0; j < lpx; j++) {
			fread (&M[i][j], sizeof (pixel), 1, imagine);
		}
		fseek (imagine, extra_octeti, SEEK_CUR);
	}

	piese = fopen (output, "wt");

	//parcurgerea tablei de joc
	for (i = H - 1; i >= 0; i--) {
		for (j = 0; j < L; j++) {
			/* verificarea culorii patratului, 
			apoi a unghiului si a coloanei piesei */
			if (culoare_patrat (i, j, M) == 1) {
				if (albastru (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 2) {
				if (galben (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 3) {
				if (rosu (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 4) {
				if (verde (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 5) {
				if (portocaliu (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 6) {
				if (roz (i, j, piese, M)) {
					numar++;
				}
			} else if (culoare_patrat (i, j, M) == 7) {
				if (mov (i, j, piese, M)) {
					numar++;
				}
			}
		}
	}

	fprintf(piese, "%d\n", numar);
	for (i = 0; i < hpx; i++) {
		free (M[i]);
	}
	free (M);
	fclose (imagine);
	fclose (piese);
}