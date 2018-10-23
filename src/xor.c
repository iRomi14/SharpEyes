#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "matrix/matrix.h"

#define EPOCHS 1000

#define saveFile "weights.se"

typedef struct NeuralNet {
  Matrix *weights;
  Matrix *part_d;
  size_t layers;
} NN;

Matrix forward(NN neuralNet, Matrix X){
  Matrix y = X;
  for(size_t i = 0; i < neuralNet.layers; i++){
    y = sigmoidMatrix(dotMatrix(y, neuralNet.weights[i]), false);
    neuralNet.part_d[i] = y;
  }

  return y;
}

void backward(NN neuralNet, Matrix error, Matrix X){

  Matrix delta, prev;



  for(int i = (int)neuralNet.layers-1; i >= 0; i--){
    if(i > 0)
      prev = neuralNet.part_d[i-1];
    else
      prev = X;

    delta = multMatrix(error,sigmoidMatrix(neuralNet.part_d[i], true));
    error = dotMatrix(delta, transpose(neuralNet.weights[i]));

    neuralNet.weights[i] = addMatrix(neuralNet.weights[i], dotMatrix(transpose(prev), delta));
    
  }
}

void saveWeights(NN neuralNet){
	FILE *fp;
	fp=fopen(saveFile, "w");

	for(size_t i = 0; i < neuralNet.layers; i++){
	    writeMatrix(fp, neuralNet.weights[i]);
	}

	fclose(fp);
}


int main(){
  srand (time(NULL));

  /*
   * X = [[0,0],
   *      [0,1],
   *      [1,0],
   *      [1,1]]
   */
  Matrix X = initMatrix(4, 2, false);
  X.data[0].data[0] = 0.0;
  X.data[0].data[1] = 0.0;
  X.data[1].data[0] = 1.0;
  X.data[1].data[1] = 0.0;
  X.data[2].data[0] = 0.0;
  X.data[2].data[1] = 1.0;
  X.data[3].data[0] = 1.0;
  X.data[3].data[1] = 1.0;

  /*
   * y = xor(X) = [[0],
   *               [1],
   *               [1],
   *               [0]]
   */
  Matrix y = initMatrix(4, 1, false);
  y.data[0].data[0] = 0.0;
  y.data[1].data[0] = 1.0;
  y.data[2].data[0] = 1.0;
  y.data[3].data[0] = 0.0;

  
  NN xorNet;
  xorNet.layers = 2;

  xorNet.weights = (Matrix *) calloc (xorNet.layers, sizeof(Matrix));
  xorNet.part_d = (Matrix *) calloc (xorNet.layers, sizeof(Matrix));

  xorNet.weights[0] = initMatrix(2, 4, true);
  xorNet.weights[1] = initMatrix(4, 1, true);

  Matrix y_pred;
  Matrix error;

  int j;
  for(j = 0; j < EPOCHS; j++){
    y_pred = forward(xorNet, X);

    error = addMatrix(y, scalarMatrix(y_pred, -1.0)); // y - l2

    //printMatrix(l2);
    //printf("-----------------\n");
    
    if (j% 100 == 0)
      printf ("Cost at %d epochs : %lf\n", j, meanMatrix(error));


    backward(xorNet, error, X);

  }

  printf("---------Output---------\n");

  Matrix x = initMatrix(1, 2, false);
  x.data[0].data[0] = 0.0;
  x.data[0].data[1] = 0.0;
  printf("Xor(0, 0) = %lf ( expected : %lf )\n", forward(xorNet, x).data[0].data[0], y.data[0].data[0]);

  x.data[0].data[0] = 1.0;
  x.data[0].data[1] = 0.0;
  printf("Xor(1, 0) = %lf ( expected : %lf )\n", forward(xorNet, x).data[0].data[0], y.data[1].data[0]);

  x.data[0].data[0] = 0.0;
  x.data[0].data[1] = 1.0;
  printf("Xor(0, 1) = %lf ( expected : %lf )\n", forward(xorNet, x).data[0].data[0], y.data[2].data[0]);

  x.data[0].data[0] = 1.0;
  x.data[0].data[1] = 1.0;
  printf("Xor(1, 1) = %lf ( expected : %lf )\n", forward(xorNet, x).data[0].data[0], y.data[3].data[0]);


  saveWeights(xorNet);

  printf("Weights saved in %s\n", saveFile);

  return 0;
}
