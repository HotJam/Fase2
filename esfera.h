#ifndef Esfera_H
#define Esfera_H
#include "vertice.h"

typedef struct esfera {

	int n_vertices;
	int div_horizontais;
	int div_verticais;

	vertice *conjunto_vertices;
};

#endif
