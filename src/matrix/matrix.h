#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Matrix {
	size_t shape[2];
	Vector* data;
};

typedef struct Matrix Matrix;

Matrix initMatrix(size_t l, size_t m, bool random);

void printMatrix(Matrix m);
void freeMatrix(Matrix m);

Matrix addMatrix(Matrix a, Matrix b);

Matrix scalarMatrix(Matrix m, double s);

Matrix multMatrix(Matrix a, Matrix b);

Matrix dotMatrix(Matrix a, Matrix b);

Matrix transpose(Matrix m);

Matrix sigmoidMatrix(Matrix m, bool deriv);

//Vector absVector(Vector v);
//Matrix absMatrix(Matrix m);

double meanMatrix(Matrix m);

#endif
