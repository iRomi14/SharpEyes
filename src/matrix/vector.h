#ifndef VECTOR_H
#define VECTOR_H

#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Vector {
	size_t size;
	double *data;
};

typedef struct Vector Vector;


void initVector(Vector *dst, size_t size, bool random);
void loadVector(Vector *dst, char *in, size_t size);

void printVector(Vector v);
void writeVector(FILE *out, Vector v);
void freeVector(Vector v);

void addVector(Vector *dst, Vector a, Vector b);
void scalarVector(Vector *dst, Vector v, double s);

void multVector(Vector *dst, Vector a, Vector b);

void sigmoidVector(Vector *dst, Vector v, bool deriv);

double meanVector(Vector v);

int argmax(Vector v);

#endif
