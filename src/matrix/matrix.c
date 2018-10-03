#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "matrix.h"

Vector initVector(unsigned int size, bool random){

  Vector v;
  v.size = size;
  v.data = (double*) calloc(size, sizeof(double));
  
  if(random){
    int i;
    for(i = 0; i < size; i++){
      v.data[i] = (double)rand()/RAND_MAX*2.0-1.0;;
    }
  }
  
  return v;
}

void printVector(Vector v){
  int i;
  printf("[ ");
  for(i = 0; i < v.size; i++){
    printf("%lf ", v.data[i]);
  }
  printf("]\n");
}

Matrix initMatrix(unsigned int l, unsigned int m, bool random){
  Matrix M;
  M.shape[0] = l;
  M.shape[1] = m;
  M.data = (Vector*) calloc (l, sizeof(Vector));

  int i;
  for(i = 0; i < l; i++){
    M.data[i] = initVector(m, random);
  }

  return M;
}

void printMatrix(Matrix m){
  int i;
  for(i = 0; i < m.shape[0]; i++){
    printVector(m.data[i]);
  }
}

void freeVector(Vector v){
  free(v.data);
}
void freeMatrix(Matrix m){
  int i;
  for(i = 0; i < m.shape[0]; i++){
    freeVector(m.data[i]);
  }
}


Vector addVector(Vector a, Vector b){
  if(a.size != b.size)
    return initVector(0, false);

  unsigned int size = a.size;
  Vector res = initVector(size, false);
  int i;
  for(i = 0; i < size; i++){
    res.data[i] = a.data[i] + b.data[i];
  }

  return res;
}


Matrix addMatrix(Matrix a, Matrix b){
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1])
    return initMatrix(0, 0, false);

  Matrix res = initMatrix(a.shape[0], a.shape[1], false);
  int i;
  for(i = 0; i < a.shape[0]; i++){
    res.data[i] = addVector(a.data[i], b.data[i]);
  }

  return res;
}



Vector scalarVector(Vector v, double s){
  Vector res = initVector(v.size, false);
  int i;
  for(i = 0; i < v.size; i++){
    res.data[i] = s*v.data[i];
  }

  return res;
}


Matrix scalarMatrix(Matrix m, double s){
  Matrix res = initMatrix(m.shape[0], m.shape[1], false);
  int i;
  for(i = 0; i < m.shape[0]; i++){
    res.data[i] = scalarVector(m.data[i], s);
  }

  return res;
}

Vector multVector(Vector a, Vector b){
  if(a.size != b.size)
    return initVector(0, false);

  unsigned int size = a.size;
  Vector res = initVector(size, false);
  int i;
  for(i = 0; i < size; i++){
    res.data[i] = a.data[i] * b.data[i];
  }

  return res;
}

Matrix multMatrix(Matrix a, Matrix b){
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1])
    return initMatrix(0, 0, false);

  Matrix res = initMatrix(a.shape[0], a.shape[1], false);
  int i;
  for(i = 0; i < a.shape[0]; i++){
    res.data[i] = multVector(a.data[i], b.data[i]);
  }

  return res;
}

Matrix dotMatrix(Matrix a, Matrix b){
  if (a.shape[1] != b.shape[0])
    return initMatrix(0, 0, false);

  unsigned int l = a.shape[0];
  unsigned int m = b.shape[1]; 
  Matrix res = initMatrix(l, m, false);
  int i, j, k;

  Vector v;
  for(i = 0; i < l; i++){
    v = res.data[i];
    for(j = 0; j < m; j++){
      for(k = 0; k < a.shape[1]; k++){
        v.data[j] += a.data[i].data[k] * b.data[k].data[j];
      }
    } 
  }

  return res;
}

Matrix transpose(Matrix m){
  unsigned int l = m.shape[0];
  unsigned int k = m.shape[1]; 
  Matrix res = initMatrix(k, l, false);

  int i, j;

  for(i = 0; i < l; i++){
    for(j = 0; j < k; j++){
      res.data[j].data[i] = m.data[i].data[j];
    }
  }

  return res;
}


double sigmoid(double x, bool deriv){
  if(deriv)
    return x*(1-x);
  return 1/(1+exp(-x));
}

Vector sigmoidVector(Vector v, bool deriv){
  Vector res = initVector(v.size, false);
  int i;
  for(i = 0; i < res.size; i++){
    res.data[i] = sigmoid(v.data[i], deriv);
  }

  return res;
}

Matrix sigmoidMatrix(Matrix m, bool deriv){
  Matrix res = initMatrix(m.shape[0], m.shape[1], false);
  int i;
  for(i = 0; i < res.shape[0]; i++){
    res.data[i] = sigmoidVector(m.data[i], deriv);
  }

  return res;
}


double meanVector(Vector v){
  int i;
  double mean = 0.0;
  for(i = 0; i < v.size; i++){
    mean += fabs(v.data[i]);
  }
  return mean/v.size;
}

double meanMatrix(Matrix m){
  int i;
  double mean = 0.0;
  for(i = 0; i < m.shape[0]; i++){
    mean += meanVector(m.data[i]);
  }
  return mean/m.shape[0];
}