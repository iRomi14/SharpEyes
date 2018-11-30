#ifndef SDL_functions_H
#define SDL_functions_H

# include "../pixel/pixel_operations.h"
# include <SDL.h>

void SDL_ExitError(const char *message);
void SDL_ExitSupress(const char *message, SDL_Renderer *renderer, SDL_Window *fenetre);
void PressedKey(void);
SDL_Surface* copy_image(SDL_Surface *img);
SDL_Surface* Resize(SDL_Surface *img,int width, int height);

#endif
