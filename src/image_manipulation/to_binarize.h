#ifndef TO_BINARIZE_H
#define TO_BINARIZE_H

void to_binarize(SDL_Surface *image_surface);

void greyscale(SDL_Surface *img);

int getHisto(SDL_Surface *img, int* histo);

int getThreshold(int *histo, int total);

void otsu(SDL_Surface *img);

#endif
