#include <stdio.h>
#include <SDL.h>

#include <string.h>
#include "pixel/pixel_operations.h"
#include "image_manipulation/to_binarize.h"
#include "image_manipulation/SDL_functions.h"
#include "decoupage/decoupage.h"
#include "matrix/matrix.h"

#include "neural_net/nn.h"

#define ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,_"
#define saveFile "ocr_weights_2.se"

#define HLAYER1 128
#define HLAYER2 64

int main(int argc, char *argv[]){
	srand (time(NULL));

  int EPOCHS = 10000;
  size_t N = sizeof(ALPHABET)-1;
  size_t variants = 1;

  if(argc == 3){
  	EPOCHS = atoi(argv[1]);
  	variants = (size_t)atoi(argv[2]);
  }

  printf("Loading %zu Characters\n", N*variants);

  NN ocrNet;
  ocrNet.layers = 3;

  ocrNet.weights = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));
  ocrNet.part_d = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));

  initMatrix(&ocrNet.weights[0], 784, HLAYER1, true);
  initMatrix(&ocrNet.weights[1], HLAYER1, HLAYER2, true);
	initMatrix(&ocrNet.weights[2], HLAYER2, N, true);
  //initMatrix(&xorNet.weights[1], 64, , true);

  SDL_Surface *image, *processed;

  char buffer[64];

  char train_dir[] = "Banque Image/training/";
  char image_name[] = "x/xx.bmp";

  Matrix train_x;
  Matrix train_y;

  initMatrix(&train_x, N*variants, 784, false);
  initMatrix(&train_y, N*variants, N, false);

  Vector v;

  for (size_t i = 0; i < N; i++){
    //image_name[0] = ALPHABET[i];

    //printf("%s\n", strncat(buffer, image_name, 64));
    for(size_t j = 0; j < variants; j++){

      sprintf(image_name, "%c/%02zu.bmp", ALPHABET[i], j);
      strcpy(buffer, train_dir);

      //printf("%s\n", strncat(buffer, image_name, 64));
      image = SDL_LoadBMP(strncat(buffer, image_name, 64));
      if (image == NULL)
        exit(-1);

      train_y.data[i*variants+j].data[i] = 1.0; // one hot encoding
      to_binarize(image);
			processed = draw_lines(image);
			processed = isolateLine(processed);
			//SDL_SaveBMP(image, "./draw_lines.bmp");
			//double *letter = create_matrix(processed);
			//print_matrix(letter, 28, 28);
      bmp_to_vector(&v, processed);

      train_x.data[i*variants+j] = v;
    }
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
