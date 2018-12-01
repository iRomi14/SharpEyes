#ifndef SMOOTH
#define SMOOTH

void remove_dots(SDL_Surface *img);
int neighbors(SDL_Surface *img, int i, int j, int w, int h);
int black_or_white(SDL_Surface *img, int *tab);

#endif