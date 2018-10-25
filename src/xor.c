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

Matrix forward(NN neuralNet, Matrix in){
  Matrix out = in;
  Matrix tmp;
  for(size_t i = 0; i < neuralNet.layers; i++){
    /*  prev_layer = X
        next_layer = sigmoid(prev_layer *dot* weights[i] );
        prev_layer = next_layer
    */
    dotMatrix(&tmp, out, neuralNet.weights[i]);
    sigmoidMatrix(&out, tmp, false);
    neuralNet.part_d[i] = out;
    freeMatrix(tmp);
  }

  return out;
}

void backward(NN neuralNet, Matrix error, Matrix in){

  Matrix delta, prev;
  Matrix tmp, gradW;

  for(int i = (int)neuralNet.layers-1; i >= 0; i--){ // backpropagation

    if(i > 0)
      prev = neuralNet.part_d[i-1];
    else
      prev = in;

    // delta = error * sigmoid'(layer);
    sigmoidMatrix(&gradW, neuralNet.part_d[i], true);
    multMatrix(&delta, error, gradW);
    freeMatrix(gradW);

    // prev_error = delta *dot* weights[i].T;
    transpose(&tmp, neuralNet.weights[i]);
    dotMatrix(&error, delta, tmp);
    freeMatrix(tmp);

    // weights[i] += delta *dot* prev_layer.T
    transpose(&tmp, prev);
    dotMatrix(&gradW, tmp, delta);
    addMatrix(&neuralNet.weights[i], neuralNet.weights[i], gradW);
  }
}

void freeNeuralNet(NN neuralNet){
  for(size_t i = 0; i < neuralNet.layers; i++){
    freeMatrix(neuralNet.weights[i]);
    freeMatrix(neuralNet.part_d[i]);
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
  Matrix X;
  initMatrix(&X, 4, 2, false);
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
  Matrix y;
  initMatrix(&y, 4, 1, false);
  y.data[0].data[0] = 0.0;
  y.data[1].data[0] = 1.0;
  y.data[2].data[0] = 1.0;
  y.data[3].data[0] = 0.0;


  NN xorNet;
  xorNet.layers = 2;

  xorNet.weights = (Matrix *) calloc (xorNet.layers, sizeof(Matrix));
  xorNet.part_d = (Matrix *) calloc (xorNet.layers, sizeof(Matrix));

  initMatrix(&xorNet.weights[0], 2, 4, true);
  initMatrix(&xorNet.weights[1], 4, 1, true);

  Matrix y_pred, error;
  Matrix tmp;

  int j;
  for(j = 0; j < EPOCHS; j++){
    y_pred = forward(xorNet, X);

    scalarMatrix(&tmp, y_pred, -1.0);
    addMatrix(&error, y, tmp); // y - y_pred
    freeMatrix(tmp);


    //printMatrix(l2);
    //printf("-----------------\n");

    if (j% 100 == 0)
      printf ("Cost at %d epochs : %lf\n", j, meanMatrix(error));


    backward(xorNet, error, X);

  }

  freeMatrix(X);
  freeMatrix(y_pred);

  printf("---------Output---------\n");

  Matrix x;
  initMatrix(&x, 1, 2, false);
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

  freeMatrix(x);
  freeMatrix(y);

  saveWeights(xorNet);

  printf("Weights saved in %s\n", saveFile);

  freeNeuralNet(xorNet);

  return 0;
}
