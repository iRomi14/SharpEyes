#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <math.h>

struct Vector {
	size_t size;
	double *data;
};

typedef struct Vector Vector;


Vector initVector(size_t size, bool random);

void printVector(Vector v);
void freeVector(Vector v);

Vector addVector(Vector a, Vector b);
Vector scalarVector(Vector v, double s);

Vector multVector(Vector a, Vector b);

Vector sigmoidVector(Vector v, bool deriv);

double meanVector(Vector v);

#endif
