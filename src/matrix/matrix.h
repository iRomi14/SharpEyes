#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <math.h>

struct Vector {
	unsigned int size;
	double* data;
};

typedef struct Vector Vector;

struct Matrix {
	unsigned int shape[2];
	Vector* data;
};

typedef struct Matrix Matrix;

Vector initVector(unsigned int size, bool random);
Matrix initMatrix(unsigned int l, unsigned int m, bool random);

void printVector(Vector v);
void printMatrix(Matrix m);

void freeVector(Vector v);
void freeMatrix(Matrix m);


Vector addVector(Vector a, Vector b);
Matrix addMatrix(Matrix a, Matrix b);

Vector scalarVector(Vector v, double s);
Matrix scalarMatrix(Matrix m, double s);

Vector multVector(Vector a, Vector b);
Matrix multMatrix(Matrix a, Matrix b);

Matrix dotMatrix(Matrix a, Matrix b);

Matrix transpose(Matrix m);

Vector sigmoidVector(Vector v, bool deriv);
Matrix sigmoidMatrix(Matrix m, bool deriv);

//Vector absVector(Vector v);
//Matrix absMatrix(Matrix m);

double meanVector(Vector v);
double meanMatrix(Matrix m);

#endif