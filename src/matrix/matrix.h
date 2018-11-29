#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Matrix {
	size_t shape[2];
	Vector* data;
};

typedef struct Matrix Matrix;

void initMatrix(Matrix *dst, size_t l, size_t m, bool random);
void loadMatrix(Matrix *dst, char *in);

void printMatrix(Matrix m);
void writeMatrix(FILE *out, Matrix m);
void freeMatrix(Matrix m);

void addMatrix(Matrix *dst, Matrix a, Matrix b);

void scalarMatrix(Matrix *dst, Matrix m, double s);

void multMatrix(Matrix *dst, Matrix a, Matrix b);

void dotMatrix(Matrix *dst, Matrix a, Matrix b);

void transpose(Matrix *dst, Matrix m);

void sigmoidMatrix(Matrix *dst, Matrix m, bool deriv);

double meanMatrix(Matrix m);

#endif
