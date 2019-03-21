#ifndef Caixa_H
#define Caixa_H
#include "vertice.h"

typedef struct caixa {

	int n_vertices;
	int divisoes;

	vertice *conjunto_vertices;
};

#endif