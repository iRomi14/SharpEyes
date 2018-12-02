#include "reconstruct.h"
#define MIN_SZ 32


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

void initText(){
  Final_Text.size = MIN_SZ;
  Final_Text.idx = 0;
  Final_Text.data = (char *)calloc(MIN_SZ, sizeof(char));
}

void addChar(char c){
  if(Final_Text.idx+1 >= Final_Text.size){
    Final_Text.size += MIN_SZ;
    Final_Text.data = realloc(Final_Text.data, Final_Text.size);
    if(Final_Text.data == NULL)
      exit(-1);
  }
  Final_Text.data[Final_Text.idx] = c;
  Final_Text.idx++;
  Final_Text.data[Final_Text.idx] = '\0';
}

void OCR_recon(SDL_Surface *image_surface){

  Matrix x, y_pred;
  Vector v;

  bmp_to_vector(&v, image_surface);
  initMatrix(&x, 1, v.size, false);
  x.data[0] = v;

  y_pred = forward(ocrNet, x);

  addChar(ALPHABET[argmax(y_pred.data[0])]);

  freeMatrix(x);
}
