#ifndef NN_H
#define NN_H

#include "../matrix/matrix.h"

typedef struct NeuralNet {
  Matrix *weights;
  Matrix *part_d;
  size_t layers;
} NN;

Matrix forward(NN neuralNet, Matrix in);

void backward(NN neuralNet, Matrix error, Matrix in);

void freeNeuralNet(NN neuralNet);

void saveWeights(NN neuralNet, char *saveFile);

void loadWeights(NN *neuralNet, char *loadFile);

#endif
