#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

//test

struct Vector {
	unsigned int size;
	double* data;
};

typedef struct Vector Vector;

Vector* initVector(unsigned int size, bool random){
	Vector *v;
	v = (Vector *) malloc(sizeof(Vector));

	v->size = size;
	v->data = (double *) calloc(size, sizeof(double));

	if(random){
    int i;
    for(i = 0; i < size; i++){
      v->data[i] = (double)rand()/RAND_MAX*2.0-1.0;;
    }
  }

  return v;
}

void printVector(Vector* v){
  int i;
  printf("[ ");
  for(i = 0; i < v->size; i++){
    printf("%lf ", v->data[i]);
  }
  printf("]\n");
}


int main(){
  srand ( time ( NULL));

  Vector *v1;

  v1 = initVector(10, true);
  printVector(v1);
}
