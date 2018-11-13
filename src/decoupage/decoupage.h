#ifndef DECOUPAGE_H
#define DECOUPAGE_H

# include "../matrix/matrix.h"

SDL_Surface* lines(SDL_Surface* img);
SDL_Surface* columns(SDL_Surface *img, int b_line, int e_line);

#endif
