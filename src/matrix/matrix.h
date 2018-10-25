#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Matrix {
	size_t shape[2];
	Vector* data;
};

typedef struct Matrix Matrix;

void initMatrix(Matrix *des, size_t l, size_t m, bool random);

void printMatrix(Matrix m);
void writeMatrix(FILE *out, Matrix m);
void freeMatrix(Matrix m);

void addMatrix(Matrix *dst, Matrix a, Matrix b);

void scalarMatrix(Matrix *dst, Matrix m, double s);

void multMatrix(Matrix *dst, Matrix a, Matrix b);

void dotMatrix(Matrix *dst, Matrix a, Matrix b);

void transpose(Matrix *dst, Matrix m);

void sigmoidMatrix(Matrix *dst, Matrix m, bool deriv);

//Vector absVector(Vector v);
//Matrix absMatrix(Matrix m);

double meanMatrix(Matrix m);

#endif
