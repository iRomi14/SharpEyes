#include <stdio.h>
#include <SDL.h>

#include <string.h>
#include "pixel/pixel_operations.h"
#include "image_manipulation/to_binarize.h"
#include "image_manipulation/SDL_functions.h"
#include "decoupage/decoupage.h"
#include "matrix/matrix.h"

#include "neural_net/nn.h"

#define ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define saveFile "ocr_weights_light.se"

#define HLAYER 64

void bmp_to_vector(Vector *dst, SDL_Surface *image_surface){

  size_t width = (size_t)image_surface->w; //je recupère la largeur de l'image soit le nombre de pixel en largeur.

  size_t height = (size_t)image_surface->h; //je recupère la hauteur de l'image soit le nombre de pixel en hauteur.

  //Matrix final;
  initVector(dst, height*width, false); // j'initialise une matrice de hauteur par largeur.

  for (size_t x = 0 ; x < width; x++) {
    for (size_t y = 0; y < height; y++) { //je parcour mes pixels un par un.

      Uint32 pixel = get_pixel(image_surface,x,y); // je recupère un pixel.
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); // je recupère ces valeur R G B.

      if (r == 255) //blanc
        dst->data[x*height+y]/*.data[x]*/ = -1.0; //si c'est un pixel blanc je met ca valeur à 1.

      if (r == 0) //noir
        dst->data[x*height+y]/*.data[x]*/  = 1.0;//si c'est un pixel noir je met ca valeur à 0.

    }
  }
}

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
  ocrNet.layers = 2;

  ocrNet.weights = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));
  ocrNet.part_d = (Matrix *) calloc (ocrNet.layers, sizeof(Matrix));

  initMatrix(&ocrNet.weights[0], 784, HLAYER, true);
  initMatrix(&ocrNet.weights[1], HLAYER, N, true);
  //initMatrix(&xorNet.weights[1], 64, , true);

  SDL_Surface *image;

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

      train_y.data[i*variants+j].data[i] = 1.0;
      to_binarize(image);
      bmp_to_vector(&v, image);

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
