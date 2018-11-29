# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "decoupage.h"
# include "../pixel/pixel_operations.h"
# include "../matrix/matrix.h"
# include "../image_manipulation/SDL_functions.h"

/*
**  SEGMENTATION
**  file: segmentation.c
**  description: file containing all that has to do with the segmentation
**  process
*/

/*
EXAMPLE :
  <-----COLUMN [FIX][X]-------->
DOWN L                            DOWN R
  {{'1','0','2','X','2','1','1'},   -
  {'X','1','1','2','2','1','1'},    !
  {'X','1','1','2','2','1','1'},    !
  {'1','X','2','X','2','2','2'},    !
  {'1','X','1','X','1','X','2'},    ROW [Y]
  {'1','X','2','X','2','1','1'},    !
  {'1','X','2','2','1','X','1'},    !
  {'1','X','2','X','2','1','X'},    !
  {'1','1','1','X','2','2','1'},    !
  {'1','X','2','X','2','1','1'}}    -
DOWN L                            DOWN R
'total' will be 70 = 10 * 7
'column' will be 7 = size of first row
'row' will be 10 = 70 / 7
*/

//******************************************************************************//
//****************************** R L S A ***************************************//
//******************************************************************************//


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
            printf("%d ", (int)mat[j + i * cols]);

	printf("]");
        printf("\n");
    }
    printf("\n");
}


//******************************************************************************//
//********************** DETECTION OF WHITE LINES ******************************//
//******************************************************************************//

/* Cut the lines of the image */
SDL_Surface* lineCut(SDL_Surface *img)
{
    /* Variables */
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlank = 1;
    int firstCut = 1;
    SDL_Surface *img_copy = copy_image(img);
    for(int i = 0; i < (img_copy -> h) ; i++)
    {
      isBlank = 1 ;
      for(int j = 0 ; j < (img_copy -> w); j++)
      {
        pixel = get_pixel(img, j, i);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        //Check if there is a black character in this line
        if(!r && !g && !b)
        {
          isBlank = 0;
          break;
        }
      }
      //For the first cut we cut the pixel line
      //before the line with a black character
      if(!isBlank && firstCut)
      {
          for(int k = 0; k < (img_copy -> w); k++)
          {
            pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);
            put_pixel(img_copy, k, i - 1, pixel);
          }
          firstCut = 0;
      }
      //For the second cut we cut the first white line
      if(isBlank && !firstCut)
      {
        for(int k = 0; k < (img_copy -> w); k++)
        {
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
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int firstCut;
  int lastCut = 0;

  for(int i = 0; i < (img -> h); i++)
  {
    if(i != 0 && i <= lastCut)
    {
      continue;
    }
    pixel = get_pixel(img, 0, i);
    SDL_GetRGB(pixel, img -> format, &r, &g, &b);
    //If there is a red line start cut
    if(r == 255 && b == 0 && g == 0)
    {
      firstCut = i + 1;

      for(int j = firstCut + 1; j < (img -> h); j++)
      {
        pixel = get_pixel(img, 0, j);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);

        //Detect the second line
        if(r == 255 && b==0 && g == 0)
        {
          lastCut = j;
          //Isolate into a surface the linecut
          cuttedSurface(img, firstCut, lastCut);
          break;
        }
      }
    }
  }
}

/* Display the isolated cuts */
void cuttedSurface(SDL_Surface *img, int firstCut,
                  int lastCut)
{
  /* Variables */
  Uint32 pixel;
  SDL_Surface* copy = NULL;

  //Create a surface that contain the cutting zone
  copy = SDL_CreateRGBSurface(0,
                              img -> w,
                              lastCut - firstCut,
                              img -> format -> BitsPerPixel, 0, 0, 0, 0);
  //Copy the cutting zone into the new surface
  for(int i = 0; i < copy -> w; i++)
  {
    for(int j = 0; j < copy -> h; j++)
    {
      pixel = get_pixel(img, i, firstCut + j);
      put_pixel(copy, i, j, pixel);
    }
  }
  //Cut the characters
  charCut(copy);
  isolateChar(copy);
}

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//

/* Cut the characters into the isolated cuts */
void charCut(SDL_Surface *img)
{
  // Variables
  Uint32 pixel;
  Uint8 r ;
  Uint8 g;
  Uint8 b;
  int thereIsChar = 0;
  int lineWasWhite = 1;
  int canCut = 0;

  for(int i = 0; i < img -> w; i++)
  {
    lineWasWhite = 1;
    for(int j = 0; j < img -> h; j++)
    {
      pixel = get_pixel(img, i, j);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 0 && g == 0 && b == 0)
      {
          thereIsChar = 1;
          lineWasWhite = 0;
          break;
      }
    }
    if(lineWasWhite && !canCut)
    {
      continue;
    }
    if(thereIsChar && !canCut)
    {
      for(int k = 0; k < img -> h; k++)
      {
        pixel = SDL_MapRGB(img -> format, 0, 0, 255);
        put_pixel(img, i - 1, k, pixel);
      }
      canCut = 1;
    }
    if(lineWasWhite && canCut)
    {
      for(int k = 0; k < img -> h; k++)
      {
        pixel = SDL_MapRGB(img -> format, 255, 0, 0);
        put_pixel(img, i, k, pixel);
      }
      canCut = 0;
    }

  }
}
void isolateChar(SDL_Surface *img)
{
  //Variables
  Uint32 pixel;
  Uint8 r ;
  Uint8 g;
  Uint8 b;
  int firstCut;
  int lastCut = -1;
  int lastRead = -1;
  SDL_Surface* copy = NULL;
  SDL_Surface *resize = NULL;

  for(int i = 0; i < img -> w; i++)
  {
    if(i < lastRead)
    {
      continue;
    }
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

      //Search for second Cut
      for(int j = firstCut + 1; j < img -> w; j++)
      {
        pixel = get_pixel(img, j, 0);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        if(r == 255 && g == 0 && b == 0)
        {
          lastRead = j;
          lastCut = j - 1;

          //Create a surface that contain the cutting zone
          copy = SDL_CreateRGBSurface(0,
                                      lastCut-firstCut + 1,
                                      img->h,
                                      img->format->BitsPerPixel,0,0,0,0);
          //Copy the cutting zone into the new surface
          for(int w = 0; w < copy -> w; w++)
          {
            for(int h = 0; h < copy -> h; h++)
            {
              pixel = get_pixel(img, firstCut + w, h);
              put_pixel(copy, w, h, pixel);
            }
          }
          resize = increaseChar(copy);
          //Detect the char
          Vector v;
          bmp_to_vector(&v, resize);
          printVector(v);

	  //double *letter = create_matrix(resize);
	  //print_matrix(letter, 28, 28);
          //Prevent false space
          /*int space = 1;
          //printf("%d\n",copy->w);
          if(copy->w <5)
          {
            for(int i = 0;i < 28*28; i++)
            {
              if((int)letter[i] == 1)
              {
                space = 0;
                break;
              }
            }
            if(space == 1)
              break;
          }

          space = 1;
          for(int i = 0;i < 28*28; i++)
          {
            if((int)letter[i] == 1)
            {
              space = 0;
              break;
            }
	  }*/
          /*if(space)
          */
          break;
        }
      }
    }
  }
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
