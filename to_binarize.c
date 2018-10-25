#include "pixel/pixel_operations.h"
#include <SDL.h>

void to_binarize(SDL_Surface *image_surface)
{
  int width = image_surface->w;
  int height = image_surface->h;

  for (int x = 0 ; x < width; x++) {
    for (int y = 0; y < height; y++) {
      Uint32 pixel = get_pixel(image_surface,x,y);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      Uint8 average = 0.3*r + 0.59*g + 0.11*b;

      if (average <= 127)
        r = g = b = 0;

      if (average >127)
        r = g = b = 255;

      Uint32 new_pixel = SDL_MapRGB(image_surface->format, r, g, b);
      put_pixel(image_surface, x, y, new_pixel);
    }
  }
}
