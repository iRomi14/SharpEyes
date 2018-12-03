#ifndef NN_H
#define NN_H

#include "../matrix/matrix.h"

#define ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,."
#define train_dir "res/training/"

typedef struct NeuralNet {
  Matrix *weights;
  Matrix *part_d;
  size_t layers;
  bool train;
} NN;

Matrix forward(NN neuralNet, Matrix in);

void backward(NN neuralNet, Matrix error, Matrix in);

void freeNeuralNet(NN neuralNet);

void saveWeights(NN neuralNet, char *saveFile);

void loadWeights(NN *neuralNet, char *loadFile);

#endif
