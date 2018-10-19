#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "vector.h"

Vector initVector(size_t size, bool random){

  Vector v;
  v.size = size;
  v.data = (double*) calloc(size, sizeof(double));
  
  if(random){
    for(size_t i = 0; i < size; i++){
      v.data[i] = (double)rand()/RAND_MAX*2.0-1.0;;
    }
  }
  
  return v;
}

void printVector(Vector v){
  printf("[ ");
  for(size_t i = 0; i < v.size; i++){
    printf("%lf ", v.data[i]);
  }
  printf("]\n");
}

void freeVector(Vector v){
  free(v.data);
}

Vector addVector(Vector a, Vector b){
  if(a.size != b.size)
    return initVector(0, false);

  size_t size = a.size;
  Vector res = initVector(size, false);

  for(size_t i = 0; i < size; i++){
    res.data[i] = a.data[i] + b.data[i];
  }
  
  return res;
}


Vector scalarVector(Vector v, double s){
  Vector res = initVector(v.size, false);
  
  for(size_t i = 0; i < v.size; i++){
    res.data[i] = s*v.data[i];
  }
  
  return res;
}

Vector multVector(Vector a, Vector b){
  if(a.size != b.size)
    return initVector(0, false);

  size_t size = a.size;
  Vector res = initVector(size, false);

  for(size_t i = 0; i < size; i++){
    res.data[i] = a.data[i] * b.data[i];
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

  for(size_t i = 0; i < res.size; i++){
    res.data[i] = sigmoid(v.data[i], deriv);
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

