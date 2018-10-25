#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct Vector {
	size_t size;
	double *data;
};

typedef struct Vector Vector;


void initVector(Vector *dst, size_t size, bool random);

void printVector(Vector v);
void writeVector(FILE *out, Vector v);
void freeVector(Vector v);

/*	Matrix Operators
	reuse = true : b = a operator b
	reuse = false : c = a operator b (attention a free les matrices)
*/

void addVector(Vector *dst, Vector a, Vector b);
void scalarVector(Vector *dst, Vector v, double s);

void multVector(Vector *dst, Vector a, Vector b);

void sigmoidVector(Vector *dst, Vector v, bool deriv);

double meanVector(Vector v);

#endif
