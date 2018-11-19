#include "matrix.h"

void initMatrix(Matrix *dst, size_t l, size_t m, bool random){
  dst->shape[0] = l;
  dst->shape[1] = m;
  dst->data = (Vector*) calloc (l, sizeof(Vector));

  Vector v;
  for(size_t i = 0; i < l; i++){
    initVector(&v, m, random);
    dst->data[i] = v;
  }
}

void loadMatrix(Matrix *dst, char *in){
  char *token;

  size_t l = (size_t)strtoul(in, &token, 10);
  size_t m = (size_t)strtoul(token+1, &token, 10);

  printf("%zu x %zu\n", l, m);

  initMatrix(dst, l, m, false);

  token = strtok_r(in, "\n", &in);

  Vector v;
  for(size_t i = 0; i < l; i++){
    token = strtok_r(in, "\n", &in);

    freeVector(dst->data[i]);
    loadVector(&v, token, m);
    //printf("\n");
    dst->data[i] = v;
  }
}

void printMatrix(Matrix m){
  for(size_t i = 0; i < m.shape[0]; i++){
    printVector(m.data[i]);
  }
}

void writeMatrix(FILE *out, Matrix m){
  fprintf(out, "%zux%zu {\n", m.shape[0], m.shape[1]);
  for(size_t i = 0; i < m.shape[0]; i++){
    writeVector(out, m.data[i]);
  }
  fprintf(out, "}");
}

void freeMatrix(Matrix m){
  for(size_t i = 0; i < m.shape[0]; i++){
    freeVector(m.data[i]);
  }
  free(m.data);
}

void addMatrix(Matrix *dst, Matrix a, Matrix b){
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]){
    printf("addMatrix error\n");
    return;
  }

  initMatrix(dst, a.shape[0], a.shape[1], false);
  Vector v;
  for(size_t i = 0; i < a.shape[0]; i++){
    freeVector(dst->data[i]);
    addVector(&v, a.data[i], b.data[i]);
    dst->data[i] = v;
  }
}


void scalarMatrix(Matrix *dst, Matrix m, double s){
  initMatrix(dst, m.shape[0], m.shape[1], false);
  Vector v;
  for(size_t i = 0; i < m.shape[0]; i++){
    freeVector(dst->data[i]);
    scalarVector(&v, m.data[i], s);
    dst->data[i] = v;
  }
}

void multMatrix(Matrix *dst, Matrix a, Matrix b){
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]){
    printf("multMatrix error\n");
    return;
  }

  initMatrix(dst, a.shape[0], a.shape[1], false);
  Vector v;
  for(size_t i = 0; i < a.shape[0]; i++){
    freeVector(dst->data[i]);
    multVector(&v, a.data[i], b.data[i]);
    dst->data[i] = v;
  }
}

void dotMatrix(Matrix *dst, Matrix a, Matrix b){
  if (a.shape[1] != b.shape[0]){
    printf("dotMatrix error\n");
    return;
  }

  size_t l = a.shape[0];
  size_t m = b.shape[1];
  initMatrix(dst, l, m, false);
  size_t i, j, k;

  Vector v;
  for(i = 0; i < l; i++){
    v = dst->data[i];
    for(j = 0; j < m; j++){
      for(k = 0; k < a.shape[1]; k++){
        v.data[j] += a.data[i].data[k] * b.data[k].data[j];
      }
    }
  }
}

void transpose(Matrix *dst, Matrix m){
  size_t l = m.shape[0];
  size_t k = m.shape[1];
  initMatrix(dst, k, l, false);

  size_t i, j;

  for(i = 0; i < l; i++){
    for(j = 0; j < k; j++){
      dst->data[j].data[i] = m.data[i].data[j];
    }
  }
}


void sigmoidMatrix(Matrix *dst, Matrix m, bool deriv){
  initMatrix(dst, m.shape[0], m.shape[1], false);

  Vector v;
  for(size_t i = 0; i < m.shape[0]; i++){
    freeVector(dst->data[i]);
    sigmoidVector(&v, m.data[i], deriv);
    dst->data[i] = v;
  }
}


double meanMatrix(Matrix m){
  double mean = 0.0;
  for(size_t i = 0; i < m.shape[0]; i++){
    mean += meanVector(m.data[i]);
  }
  return mean/m.shape[0];
}
