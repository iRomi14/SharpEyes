#ifndef DECOUPAGE_H
#define DECOUPAGE_H

# include "../matrix/matrix.h"

//*********************************************************************//
//*************************** TOOLS ***********************************//
//*********************************************************************//

SDL_Surface* copy_image(SDL_Surface *img);

double *create_matrix(SDL_Surface *image_surface);

void print_matrix(double mat[], size_t lines, size_t cols);

SDL_Surface* increaseChar(SDL_Surface *img);

SDL_Surface* Resize(SDL_Surface *img);

//*********************************************************************//
//******************* DETECTION OF WHITE LINES ************************//
//*********************************************************************//

SDL_Surface* lineCut(SDL_Surface *img);

void isolateLine(SDL_Surface *img);

void cuttedSurface(SDL_Surface *img, int firstCut, int lastCut);

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//

void charCut(SDL_Surface *img);

void isolateChar(SDL_Surface *img);

#endif
