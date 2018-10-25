#include "nn.h"

Matrix forward(NN neuralNet, Matrix in){
  Matrix out = in;
  Matrix tmp;
  for(size_t i = 0; i < neuralNet.layers; i++){
    /*  prev_layer = X
        next_layer = sigmoid(prev_layer *dot* weights[i] );
        prev_layer = next_layer
    */
    freeMatrix(neuralNet.part_d[i]);

    dotMatrix(&tmp, out, neuralNet.weights[i]);
    sigmoidMatrix(&out, tmp, false);
    neuralNet.part_d[i] = out;

    freeMatrix(tmp);
  }
  return out;
}


/* Back propagation */
void backward(NN neuralNet, Matrix error, Matrix in){

  Matrix delta, prev;
  Matrix tmp, gradW;

  for(int i = (int)neuralNet.layers-1; i >= 0; i--){ 

    if(i > 0)
      prev = neuralNet.part_d[i-1];
    else
      prev = in;

    // delta = error * sigmoid'(layer);
    sigmoidMatrix(&gradW, neuralNet.part_d[i], true);
    multMatrix(&delta, error, gradW);
    freeMatrix(gradW);

    /* keep the error of the last layer (y prediction) */
    if(i < (int)neuralNet.layers-1)
      freeMatrix(error);

    // prev_error = delta *dot* weights[i].T;
    transpose(&tmp, neuralNet.weights[i]);
    dotMatrix(&error, delta, tmp);
    freeMatrix(tmp);

    // weights[i] += delta *dot* prev_layer.T
    transpose(&tmp, prev);
    dotMatrix(&gradW, tmp, delta);
    freeMatrix(tmp);

    addMatrix(&tmp, neuralNet.weights[i], gradW);
    freeMatrix(neuralNet.weights[i]);

    neuralNet.weights[i] = tmp;
    freeMatrix(gradW);
    freeMatrix(delta);
  }
  freeMatrix(error);
}

void freeNeuralNet(NN neuralNet){
  for(size_t i = 0; i < neuralNet.layers; i++){
    freeMatrix(neuralNet.weights[i]);
    freeMatrix(neuralNet.part_d[i]);
  }

  free(neuralNet.weights);
  free(neuralNet.part_d);
}

void saveWeights(NN neuralNet, char *saveFile){
	FILE *fp;
	fp=fopen(saveFile, "w");

	for(size_t i = 0; i < neuralNet.layers; i++){
	    writeMatrix(fp, neuralNet.weights[i]);
	}
	fclose(fp);
}
