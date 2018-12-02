#ifndef DECOUPAGE_H
#define DECOUPAGE_H

#include "reconstruct.h"

char savePath[64];

//*********************************************************************//
//*************************** TOOLS ***********************************//
//*********************************************************************//



double *create_matrix(SDL_Surface *image_surface);

void print_matrix(double mat[], size_t lines, size_t cols);

int isSpace(SDL_Surface* img);

//*********************************************************************//
//******************* DETECTION OF WHITE LINES ************************//
//*********************************************************************//

SDL_Surface* draw_lines(SDL_Surface *img);

void isolateLine(SDL_Surface *img);

void cutSurface(SDL_Surface *img, int firstCut, int lastCut);

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//

void draw_sperate_char(SDL_Surface *img);

void isolateChar(SDL_Surface *img);

#endif
