# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "decoupage.h"
# include "../pixel/pixel_operations.h"
# include "../matrix/matrix.h"
# include "../image_manipulation/SDL_functions.h"

size_t idx = 0;

/*
**  SEGMENTATION
**  file: segmentation.c
**  description: file containing all that has to do with the segmentation
**  process
*/

//******************************************************************************//
//******************************* TOOLS ****************************************//
//******************************************************************************//

double *create_matrix(SDL_Surface *img)
{
  //Variables
  double *letterMatrix = malloc(sizeof(double) * 28 * 28);
  Uint8 r;
  Uint8 g;
  Uint8 b;

  for(int i = 0; i < img -> h; i++)
  {
      for(int j = 0; j < img -> w; j++)
      {
          Uint32 pixel = get_pixel(img, j, i);
          SDL_GetRGB(pixel, img -> format, &r, &g, &b);
          if(r == 0 && g == 0 && b == 0)
              letterMatrix[j + i * img -> w] = 1;
          else
              letterMatrix[j + i * img -> w] = 0;
      }
  }
  return letterMatrix;
}

void print_matrix(double mat[], size_t lines, size_t cols)
{
    for(size_t i = 0; i < lines; i++)
    {
				printf("[");
        for(size_t j = 0; j < cols; j++)
            printf("%d", (int)mat[j + i * cols]);

	      printf("]\n");
    }
    printf("\n");
}

int isSpace(SDL_Surface* img)
{
  Uint32 pixel;
  Uint8 r, g, b;
  int space = 1;
  for (int y = 0; y< img -> h; y++) {
    for (int x = 0; x < img -> w; x++) {
      pixel = get_pixel(img, x, y);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 0 && g == 0 && b == 0)
      {
        space = 0;
        break;
      }
    }
  }
  return space;
}

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

//******************************************************************************//
//********************** DETECTION OF WHITE LINES ******************************//
//******************************************************************************//

/* Cut the lines of the image */
SDL_Surface* draw_lines(SDL_Surface *img)
{
    /* Variables */
    Uint32 pixel;
    Uint8 r, g, b;
    int isBlank = 1;
    int firstCut = 1;
    SDL_Surface *img_copy = copy_image(img);
    int height = img_copy -> h;
    int width = img_copy -> w;

    for(int i = 0; i < height ; i++){
      isBlank = 1 ;
      for(int j = 0 ; j < width; j++){
        pixel = get_pixel(img, j, i);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        if(!r && !g && !b) //Vérifie si il y a ue lettre sur cette ligne.
        {
          isBlank = 0;
          break;
        }
      }
      // Pour le premier découpage, on coupe la ligne avant la ligne avec les lettres.
      if(!isBlank && firstCut)
      {
          for(int k = 0; k < width; k++){
            pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);
            put_pixel(img_copy, k, i - 1, pixel);
          }
          firstCut = 0;
      }
      //Pour le second decoupage, on coupe la premère ligne blanche.
      if(isBlank && !firstCut)
      {
        for(int k = 0; k < width; k++){
          pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);
          put_pixel(img_copy, k, i, pixel);
        }
        firstCut = 1;
      }
    }
    return(img_copy);
}

/* Isolate the lines */
void isolateLine(SDL_Surface *img)
{
  /* Variables */
  Uint32 pixel;
  Uint8 r , g , b;
  int firstCut;
  int lastCut = 0;
  int height = img -> h;

  for(int i = 0; i < height; i++)
  {
    if(i != 0 && i <= lastCut)
      continue;

    pixel = get_pixel(img, 0, i);
    SDL_GetRGB(pixel, img -> format, &r, &g, &b);
    if(r == 255 && b == 0 && g == 0) //Si on a une ligne rouge on commence le découpage.
    {
      firstCut = i + 1;
      for(int j = firstCut + 1; j < height; j++)
      {
        pixel = get_pixel(img, 0, j);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);


        if(r == 255 && b==0 && g == 0) //Détecter la deuxième ligne.
        {
          lastCut = j;
          // One isole la ligne découpé avec les lettres (dedans) dans une surface.
          cutSurface(img, firstCut, lastCut);
          Final_Text[idx] = ' ';
          idx++;
          break;
        }
      }
    }
  }
}

/* Display the isolated cuts */
void cutSurface(SDL_Surface *img, int firstCut,int lastCut)
{
  //Créer une surface qui contient la zone a découpé;
  SDL_Surface* copy = SDL_CreateRGBSurface(0,img -> w,lastCut - firstCut,
                              img -> format -> BitsPerPixel, 0, 0, 0, 0);
  //COpié la zoné découpé dans une nouvelle surface.
  for(int i = 0; i < copy -> w; i++){
    for(int j = 0; j < copy -> h; j++){
      Uint32 pixel = get_pixel(img, i, firstCut + j);
      put_pixel(copy, i, j, pixel);
    }
  }
  //Cut the characters
  draw_sperate_char(copy);
  isolateChar(copy);
}

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//

/* Cut the characters into the isolated cuts */
void draw_sperate_char(SDL_Surface *img)
{
  /*Variables*/
  Uint32 pixel;
  Uint8 r, g, b;
  int thereIsChar = 0;
  int lineWasWhite = 1;
  int canCut = 0;

  for(int i = 0; i < img -> w; i++){
    lineWasWhite = 1;
    for(int j = 0; j < img -> h; j++){
      pixel = get_pixel(img, i, j);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 0 && g == 0 && b == 0){
          thereIsChar = 1;
          lineWasWhite = 0;
          break;
      }
    }
    if(lineWasWhite && !canCut)
      continue;

    if(thereIsChar && !canCut)
    {
      for(int k = 0; k < img -> h; k++){
        pixel = SDL_MapRGB(img -> format, 255, 0, 0);
        put_pixel(img, i - 1, k, pixel);
      }
      canCut = 1;
    }
    if(lineWasWhite && canCut)
    {
      for(int k = 0; k < img -> h; k++){
        pixel = SDL_MapRGB(img -> format, 255, 0, 0);
        put_pixel(img, i, k, pixel);
      }
      canCut = 0;
    }
  }
}

void isolateChar(SDL_Surface *img)
{
  /*Variables*/
  Uint32 pixel;
  Uint8 r, g, b;
  int firstCut;
  int lastCut = -1;
  int lastRead = -1;

  for(int i = 0; i < img -> w; i++){
    if(i < lastRead)
      continue;

    pixel = get_pixel(img, i, 0);
    SDL_GetRGB(pixel, img -> format, &r, &g, &b);
    if(r == 255 && g == 0 && b == 0)
    {
      firstCut = i + 1;
      pixel = get_pixel(img, firstCut, 0);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      while(r == 255 && g == 0 && b == 0)
      {
        firstCut++;
        pixel = get_pixel(img, firstCut, 0);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      }
      // On cherche pour le second découpage.
      for(int j = firstCut + 1; j < img -> w; j++){
        pixel = get_pixel(img, j, 0);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        if(r == 255 && g == 0 && b == 0)
        {
          lastRead = j;
          lastCut = j - 1;
          //On créer une nouvelle sruface qui contient la zone a découpé.
          SDL_Surface* copy = SDL_CreateRGBSurface(0,lastCut-firstCut + 1,img->h,
                                      img->format->BitsPerPixel,0,0,0,0);
          //On copie la zone a découpé dans une nouvelle surface.
          for(int w = 0; w < copy -> w; w++){
            for(int h = 0; h < copy -> h; h++){
              pixel = get_pixel(img, firstCut + w, h);
              put_pixel(copy, w, h, pixel);
            }
          }

          if(isSpace(copy) == 0)
          {
            SDL_Surface *resize = Resize(copy, 28, 28);
            //Détecter la lettre.
            /*Vector v;
            bmp_to_vector(&v, resize);
            printVector(v);*/
            double *letter = create_matrix(resize);
            print_matrix(letter, 28, 28);

            Final_Text[idx] = 'a';
            idx++;
          }

          if(copy -> w > 5 && isSpace(copy) == 1)
          {
            Final_Text[idx] = ' ';
            idx++;
          }
          break;
        }
      }
    }
  }
}
