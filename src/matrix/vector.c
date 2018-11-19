#include "vector.h"

void initVector(Vector *dst, size_t size, bool random){

  dst->size = size;
  dst->data = (double*) calloc(size, sizeof(double));

  if(random){
    for(size_t i = 0; i < size; i++){
      dst->data[i] = (double)rand()/RAND_MAX*2.0-1.0;;
    }
  }
}

void loadVector(Vector *dst, char *in, size_t size){
  initVector(dst, size, false);

  char *token = strtok_r(in, " ", &in);

  for(size_t i = 0; i < size; i++){
    token = strtok_r(in, " ", &in);
    //printf("%s  ", token);
    sscanf(token, "%lf", &(dst->data[i]));
  }
}

void printVector(Vector v){
  printf("[ ");
  for(size_t i = 0; i < v.size; i++){
    printf("%lf ", v.data[i]);
  }
  printf("]\n");
}

void writeVector(FILE *out, Vector v){
  fprintf(out, "[ ");
  for(size_t i = 0; i < v.size; i++){
    fprintf(out, "%lf ", v.data[i]);
  }
  fprintf(out, "]\n");
}

void freeVector(Vector v){
  free(v.data);
}

void addVector(Vector *dst, Vector a, Vector b){
  if(a.size != b.size)
    return;

  size_t size = a.size;
  initVector(dst, size, false);

  for(size_t i = 0; i < size; i++){
    dst->data[i] = a.data[i] + b.data[i];
  }
}


void scalarVector(Vector *dst, Vector v, double s){
  initVector(dst, v.size, false);

  for(size_t i = 0; i < v.size; i++){
    dst->data[i] = s*v.data[i];
  }
}

void multVector(Vector *dst, Vector a, Vector b){
  if(a.size != b.size)
    return;

  size_t size = a.size;
  initVector(dst, size, false);

  for(size_t i = 0; i < size; i++){
    dst->data[i] = a.data[i] * b.data[i];
  }
}

double sigmoid(double x, bool deriv){
  if(deriv)
    return sigmoid(x, false)*(1-sigmoid(x, false));
  return 1/(1+exp(-x));
}

void sigmoidVector(Vector *dst, Vector v, bool deriv){
  initVector(dst, v.size, false);

  for(size_t i = 0; i < v.size; i++){
    dst->data[i] = sigmoid(v.data[i], deriv);
  }
}


double meanVector(Vector v){
  double mean = 0.0;
  for(size_t i = 0; i < v.size; i++){
    mean += fabs(v.data[i]);
  }
  return mean/v.size;
}
