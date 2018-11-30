#include "../pixel/pixel_operations.h"
#include <SDL.h>

//******************************************************************************//
//************************************ OTSU ************************************//
//******************************************************************************//

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

void greyscale(SDL_Surface *img)
{
    Uint32 pixel;
    Uint8 r, g, b, Lum;
    for (int i = 0; i < img->w; ++i)
    {
        for (int j = 0; j < img->h; ++j)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            Lum = (r + g + b) / 3;
            put_pixel(img, i, j, SDL_MapRGB(img->format, Lum, Lum, Lum));
        }
    }
}



int getHisto(SDL_Surface *img, int* histo)
{
    Uint32 pixel;
    Uint8 r, g, b;
    int result = 0;
    for (int i = 0; i < img->w; ++i)
    {
        for (int j = 0; j < img->h; ++j)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            histo[r] += 1;
            result++;
        }
    }
    return result;
}


int getThreshold(int *hist, int total)
{
    double q1 = 0, q2 = 0, m1 = 0, m2 = 0, w1 = 0, w2 = 0;
    double sum1 = 0, sum2 = 0, between = 0, max = 0;
    int pos = 0;
    for (int i = 0; i < 256; ++i)
    {
        sum1 += hist[i];
        sum2 += hist[256 - i];
        w1 = sum1/ total;
        w2 = sum2 / total;
        q1 += i * hist[i];
        q2 += (256 - i) * hist[256 - i];
        m1 = q1 / sum1;
        m2 = q2 / sum2;
        between = w1 * w2 * (m1 - m2) * (m1 - m2);
        if (between > max)
        {
            max = between;
            pos = i;
        }

    }
    return pos;
}

void otsu(SDL_Surface *img)
{
    greyscale(img);
    int *histo = calloc(256, sizeof(int));
    int total = getHisto(img, histo);
    int pos = getThreshold(histo, total);
    Uint32 pixel;
    Uint8 r, g, b;
    for (int i = 0; i < img->w; ++i)
    {
        for (int j = 0; j < img->h; ++j)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if (r > pos)
                put_pixel(img, i, j, SDL_MapRGB(img->format, 255,255,255));
            else
                put_pixel(img, i, j, SDL_MapRGB(img->format, 0,0,0));
        }
    }
	SDL_SaveBMP(img, "../temp/binarized.bmp");
}

//******************************************************************************//
//************************* INVERSION OF COLORS*** *****************************//
//******************************************************************************//

void inverse (SDL_Surface *img)
{
	int pixBlanc = 0;
	int pixNoir = 0;
	Uint32 pix;
    Uint8 r, g, b;
    
    for (int i = 0; i < img->w; ++i)
	{
		for (int j = 0; j < img->h; ++j)
		{
			pix = get_pixel(img, i, j);
            SDL_GetRGB(pix, img->format, &r, &g, &b);
			
			if ( r == 255)
			{
				pixBlanc += 1;
			}
			if ( r == 0)
			{
				pixNoir += 1;
			}
		}
	}

	if ( pixNoir > pixBlanc)
	{
		printf("Colors have been inversed!\n");
		for (int i = 0; i < img->w; ++i)
		{
		    for (int j = 0; j < img->h; ++j)
		                    {
            pix = get_pixel(img, i, j);
            SDL_GetRGB(pix, img->format, &r, &g, &b);

            if (r == 255)
                put_pixel(img, i, j, SDL_MapRGB(img->format, 0,0,0));
            else
            	put_pixel(img, i, j, SDL_MapRGB(img->format, 255,255,255));
        	}
        }

	}
}