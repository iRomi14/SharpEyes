# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "../pixel/pixel_operations.h"
# include "smooth.h"

//gcc smooth.c -Wall -Wextra $(sdl2-config --cflags --libs)

void remove_dots(SDL_Surface *img)
{
	int nb_color;
	/*Uint32 pixel;
	Uint8 r, g, b;*/
	for (int i = 0; i < img->w; i++)
	{
		for (int j = 0; j < img->h; j++)
		{
			/*pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);*/
			nb_color = neighbors(img, i, j, img->w, img->h);
			if (nb_color == 1)
				put_pixel(img, i, j, SDL_MapRGB(img->format, 255, 255, 255));
			if (nb_color == 0)
				put_pixel(img, i, j, SDL_MapRGB(img->format, 0, 0, 0));
		}
	}
}

int neighbors(SDL_Surface *img, int i, int j, int w, int h)
{
	int tab[7];
	Uint32 pixel;
	//Uint8 r, g, b;

	pixel = get_pixel(img, mod(i+1, w), j); tab[0] = pixel;
	pixel = get_pixel(img, mod(i-1, w), j); tab[1] = pixel;
	pixel = get_pixel(img, i, mod(j+1,h)); tab[2] = pixel;
	pixel = get_pixel(img, i, mod(j-1,h)); tab[3] = pixel;
	pixel = get_pixel(img, mod(i+1, w), mod(j+1,h)); tab[4] = pixel;
	pixel = get_pixel(img, mod(i-1, w), mod(j-1,h)); tab[5] = pixel;
	pixel = get_pixel(img, mod(i+1, w), mod(j-1,h)); tab[6] = pixel;
	pixel = get_pixel(img, mod(i-1, w), mod(j+1,h)); tab[7] = pixel;

	return black_or_white(img, tab);
}

int black_or_white(SDL_Surface *img, int *tab)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int cptw = 0; int cptb = 0;
	for (int i = 0; i < 8; ++i)
	{
		pixel = tab[i];
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		if (r == 255)
			cptw++;
		if (r == 0)
			cptb++;
	}
	if (cptb > 4)
		return 0;
	if (cptw > 4)
		return 1;
	else
		return -1;
}

int mod (int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}
