# include <stdio.h>

# include "matrix/matrix.h"

Matrix bmp_to_matrix(SDL_Surface *image_surface){

  size_t width = (size_t)image_surface->w; //je recupère la largeur de l'image soit le nombre de pixel en largeur.

  size_t height = (size_t)image_surface->h; //je recupère la hauteur de l'image soit le nombre de pixel en hauteur.

  Matrix final;
  initMatrix(&final,height, width, false); // j'initialise une matrice de hauteur par largeur.

  for (size_t x = 0 ; x < width; x++) {
    for (size_t y = 0; y < height; y++) { //je parcour mes pixels un par un.

      Uint32 pixel = get_pixel(image_surface,x,y); // je recupère un pixel.
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); // je recupère ces valeur R G B.

      if (r == 255) //blanc
        final.data[y].data[x] = -1; //si c'est un pixel blanc je met ca valeur à 1.

      if (r == 0) //noir
        final.data[y].data[x] = 1;//si c'est un pixel noir je met ca valeur à 0.
