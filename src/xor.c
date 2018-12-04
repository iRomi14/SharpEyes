#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "matrix/matrix.h"
#include "neural_net/nn.h"

#define saveFile "xor_weights.se"

int main(int argc, char *argv[]){
  srand (time(NULL));

  int EPOCHS = 10000;

  if(argc == 2){
    EPOCHS = atoi(argv[1]);
  }

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
  xorNet.train = true;
  xorNet.learning_rate = 0.8;

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

    if (j % (EPOCHS/10) == 0)
      printf ("Cost at %d epochs : %lf\n", j, meanMatrix(error));

    backward(xorNet, error, X);

    freeMatrix(error);
  }

  freeMatrix(X);

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

  saveWeights(xorNet, saveFile);

  printf("Weights saved in %s\n", saveFile);

  freeNeuralNet(xorNet);

  return 0;
}
