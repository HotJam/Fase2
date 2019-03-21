#ifndef Cone_H
#define Cone_H
#include "vertice.h"

typedef struct cone {

	int n_vertices;
	int div_horizontais;
	int div_verticais;

	vertice *conjunto_vertices;
};

#endif