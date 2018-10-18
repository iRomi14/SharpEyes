#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"

Matrix initMatrix(size_t l, size_t m, bool random){
  Matrix M;
  M.shape[0] = l;
  M.shape[1] = m;
  M.data = (Vector*) calloc (l, sizeof(Vector));

  size_t i;
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

void freeMatrix(Matrix m){
  int i;
  for(i = 0; i < m.shape[0]; i++){
    freeVector(m.data[i]);
  }
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


Matrix scalarMatrix(Matrix m, double s){
  Matrix res = initMatrix(m.shape[0], m.shape[1], false);
  int i;
  for(i = 0; i < m.shape[0]; i++){
    res.data[i] = scalarVector(m.data[i], s);
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


Matrix sigmoidMatrix(Matrix m, bool deriv){
  Matrix res = initMatrix(m.shape[0], m.shape[1], false);
  int i;
  for(i = 0; i < res.shape[0]; i++){
    res.data[i] = sigmoidVector(m.data[i], deriv);
  }

  return res;
}


double meanMatrix(Matrix m){
  int i;
  double mean = 0.0;
  for(i = 0; i < m.shape[0]; i++){
    mean += meanVector(m.data[i]);
  }
  return mean/m.shape[0];
}
