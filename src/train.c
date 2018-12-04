#include <stdio.h>
#include <SDL.h>

#include <string.h>
#include "pixel/pixel_operations.h"
#include "image_manipulation/to_binarize.h"
#include "image_manipulation/SDL_functions.h"
#include "decoupage/decoupage.h"
#include "matrix/matrix.h"

#include "neural_net/nn.h"

#define saveFile "res/ocr_weights_2.se"

#define HLAYER1 64
//#define HLAYER2 64

int main(int argc, char *argv[]){
	srand (time(NULL));

  int EPOCHS = 10000;
  size_t N;
	char alphabetImage[] = "res/training/arialAlphabet.bmp";

//	NN ocrNet;
	ocrNet.layers = 2;
	ocrNet.train = true;
	ocrNet.learning_rate = 0.08;

	ocrNet.weights = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));
	ocrNet.part_d = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));


  if(argc == 2){
  	EPOCHS = atoi(argv[1]);
  }

	printf("Loading Training Set in %s\n", alphabetImage);

	isolateLine(draw_lines(SDL_LoadBMP(alphabetImage)));

	N = Final_Text.idx;

  printf("%zu Characters Found\n", N);

	initMatrix(&ocrNet.weights[0], 784, HLAYER1, true);
	initMatrix(&ocrNet.weights[1], HLAYER1, N, true);

  SDL_Surface *image;

  char buffer[64];
  char image_name[] = "x.bmp";

  Matrix train_x;
  Matrix train_y;

  initMatrix(&train_x, N, 784, false);
  initMatrix(&train_y, N, N, false);

  Vector v;

  for (size_t i = 0; i < N; i++){

    sprintf(image_name, "%c.bmp", ALPHABET[i] != '.' ? ALPHABET[i] : '_');
    strcpy(buffer, train_dir);

    image = SDL_LoadBMP(strncat(buffer, image_name, 63));
    if (image == NULL)
      exit(-1);

    train_y.data[i].data[i] = 1.0; // one hot encoding
    to_binarize(image);
    bmp_to_vector(&v, image);

		SDL_FreeSurface(image);

    train_x.data[i] = v;
  }

  Matrix y_pred, error;
  Matrix tmp;

  printf("Training for %d Epochs\n", EPOCHS);
  printf("-------------------------\n");

  int j;
  for(j = 0; j < EPOCHS; j++){
    y_pred = forward(ocrNet, train_x);

    scalarMatrix(&tmp, y_pred, -1.0);
    addMatrix(&error, train_y, tmp);
    freeMatrix(tmp);

    if (j % (EPOCHS/10) == 0)
      printf ("Cost at %d epochs : %lf\n", j, meanMatrix(error));

    backward(ocrNet, error, train_x);

    freeMatrix(error);
  }

  freeMatrix(train_y);
  freeMatrix(train_x);

  saveWeights(ocrNet, saveFile);

  printf("Weights saved in %s\n", saveFile);

  freeNeuralNet(ocrNet);

  return 0;
}
