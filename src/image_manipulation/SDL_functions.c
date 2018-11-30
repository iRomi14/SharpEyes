# include "SDL_functions.h"

void SDL_ExitError(const char *message){
	SDL_Log("Error : %s : %s\n", message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void SDL_ExitSupress(const char *message, SDL_Renderer *renderer, SDL_Window *fenetre){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(fenetre);
	SDL_ExitError(message);
}

void PressedKey(void){
	SDL_Event event;
	do{
		SDL_PollEvent(&event);
	}while(event.type != SDL_KEYDOWN);
}

SDL_Surface* copy_image(SDL_Surface *img)
{
  Uint32 pixel;
  SDL_Surface* copy;
  copy = SDL_CreateRGBSurface(0,
                              img -> w,
                              img -> h,
                              img -> format -> BitsPerPixel,0,0,0,0);
  for(int x = 0; x < img -> w; x++)
  {
    for(int y = 0; y < img -> h; y++)
    {
      pixel = get_pixel(img, x, y);
      put_pixel(copy, x, y, pixel);
    }
  }
  return(copy);
}

SDL_Surface* Resize(SDL_Surface *img, int width, int height)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_SWSURFACE,
                        width,
                        height,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}
