#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "../pixel/pixel_operations.h"

/*effectue une rotation centrale, alloue automatiquement la mémoire*/
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle)
{
 SDL_Surface* destination;
 int i;
 int j;
 Uint32 couleur;
 int mx, my, mxdest, mydest;
 int bx, by;
 float angle_radian;
 float tcos;
 float tsin;
 double largeurdest;
 double hauteurdest;

/* détermine la valeur en radian de l'angle*/
 angle_radian = -angle * 3.14159265359 / 180.0;

/*pour éviter pleins d'appel, on stocke les valeurs*/
 tcos = cos(angle_radian);
 tsin = sin(angle_radian);

/*calcul de la taille de l'image de destination*/
 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),

 // ceil : This function returns the smallest integral value not less than x
 // fabs : This function returns the absolute value of x


/*
 * alloue la mémoire à l'espace de destination, attention,
 * la surface est de même taille
 */
 destination = SDL_CreateRGBSurface(SDL_SWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);

 /*on vérifie que la mémoire a été allouée*/
 if(destination==NULL)
  return NULL;

 /*calcul du centre des images*/
 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;

 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
/* on détermine la valeur de pixel qui correspond le mieux pour la position
 * i,j de la surface de destination */

/* on détermine la meilleure position sur la surface d'origine en appliquant
 * une matrice de rotation inverse
 */

   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));
   /* on vérifie que l'on ne sort pas des bords*/
   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     couleur = get_pixel(origine, bx, by);
     put_pixel(destination, i, j, couleur);
   }
 }

return destination;
}
