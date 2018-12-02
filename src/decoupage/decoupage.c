# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "decoupage.h"

/*
**  SEGMENTATION
**  file: segmentation.c
**  description: file containing all that has to do with the segmentation
**  process
*/

//******************************************************************************//
//******************************* TOOLS ****************************************//
//******************************************************************************//

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


//******************************************************************************//
//********************** DETECTION OF WHITE LINES ******************************//
//******************************************************************************//

/* Draw red line and cut the lines of the image */
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

/* Isolate the lines with the characters in */
void isolateLine(SDL_Surface *img)
{
  /* Variables */
  Uint32 pixel;
  Uint8 r , g , b;
  int firstCut;
  int lastCut = 0;
  int height = img -> h;

  initText();

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
          addChar('\n');
          break;
        }
      }
    }
  }
}

/* Isolate in a new surface the line with characters in and start 
the segementation of characters on it */
void cutSurface(SDL_Surface *img, int firstCut,int lastCut)
{
  //Créer une surface qui contient la zone a découpé;
  SDL_Surface* copy = SDL_CreateRGBSurface(0,img -> w,lastCut - firstCut,
                              img -> format -> BitsPerPixel, 0, 0, 0, 0);
  //Copié la zoné découpé dans une nouvelle surface.
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

/* Draw, on a new surface, the line to cut the characters to isolate characters */
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

/* Isolate the characters one by one into a new surface and send it to
the neural_net */
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
            OCR_recon(resize);
            SDL_FreeSurface(resize);
          }

          if(copy -> w > 5 && isSpace(copy) == 1)
          {
            addChar(' ');
          }
          break;
        }
      }
    }
  }
}
