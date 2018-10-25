# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "decoupage.h"
# include "../pixel/pixel_operations.h"
# include "../matrix/matrix.h"

// 25/10/2018 MAIN PROBLEM : Use array instead of the created type MatriX

/*
EXAMPLE :
  <-----COLUMN [FIX][X]-------->
DOWN L                            DOWN R
  {{'1','0','2','X','2','1','1'},   -
  {'X','1','1','2','2','1','1'},    !
  {'X','1','1','2','2','1','1'},    !
  {'1','X','2','X','2','2','2'},    !
  {'1','X','1','X','1','X','2'},    ROW [Y]
  {'1','X','2','X','2','1','1'},    !
  {'1','X','2','2','1','X','1'},    !
  {'1','X','2','X','2','1','X'},    !
  {'1','1','1','X','2','2','1'},    !
  {'1','X','2','X','2','1','1'}}    -
DOWN L                            DOWN R
'total' will be 70 = 10 * 7
'column' will be 7 = size of first row
'row' will be 10 = 70 / 7
*/

//******************************************************************************//
//****************************** R L S A ***************************************//
//******************************************************************************//

// Process an 1D array
/*void rlsa ( int *tabBin[] , size_t seuil )
{
  size_t i = 0;
  size_t taille = sizeof(*tabBin);
  size_t mem = 0;
  size_t j = 0;
  while ( i < taille)
  {
    if (tabBin[i] == 0)
    {
      while ( tabBin[i] == 0 )
      {
        mem = mem + 1;
        i = i + 1;
        if ( i >= taille )
        {
          break;
        }
      }
      if ( mem <= seuil )
      {
        i = i - mem;
        for (size_t j = i; j < i + mem; j++)
        {
          *tabBin[j] = 0;
        }
        i = j;
      }
    }
    i = i + 1;
    mem = 0;
  }
}*/

 //Process an 2D array = array of the binarised image give at the beginning
 /*void rlsaALL ( int **tabBin, int seuil )
  {
    size_t total = sizeof(tabBin);
    size_t column = sizeof(tabBin[0]);
    size_t row = total / column;

    //HORIZONTAL RUN
    for (size_t y = 0; y < column; y++)
    {
      rlsa( tabBin[(int)y], (size_t) seuil);
    }

    // VERTICAL RUN
    // DO NOT WORK
    // rlsa can't be used with this layout
    for (size_t x = 0; x < row; x++)
    {
      for (size_t y = 0; y < column; y ++)
      {
        rlsa( tabBin[(int)y][(int)x] , (size_t) seuil);
      }
    }
  }*/

//******************************************************************************//
//************** FROM BLOCK TO THE COORDINATE OF THE CORNERS *******************//
//******************************************************************************//


/*
int BlackBoxesProcessing ( int tabBB[][] ; int tabB[][]  )
{
  int total = sizeof(MatrixBinarised);
  int colum = sizeof(result[0]);
  int row = total / column;

  for ( int y = 0; y < column; y++ ) {
    for ( int x = 0; x < row; x++ ) {
      // PIXEL NOIR = 0 / PIXEL BLANC = 1
      if (tabB[y][x] == 0)  { // if there is a BLACK pixel
        //run from top L to top R
        while (tabB[x][v] == 0) {
          //ACTION
        }
        y = v;
        // run from top R to down R
        while (tabB[][v] == 0) {
          //ACTION
        }
        // calculate down L
      }
    }
  }

}

*/

//******************************************************************************//
//********************** DETECTION OF WHITE LINES ******************************//
//******************************************************************************//

Matrix matrix_ligne(Matrix enter_matrix){

  size_t row = enter_matrix.shape[0];

  Matrix final;
  initMatrix(&final,row, 1, false);

  for(size_t x = 0; x < row; x++){
    int count = 0;

    for(size_t y = 0; y < enter_matrix.data[x].size; y++){

      if (enter_matrix.data[x].data[y] == 0)
        count++;
    }

    if(count != 0){
      final.data[x].data[0] = 1;
    }

    else{
      final.data[x].data[0] = 0;
    }
  }
  return final;

}

//******************************************************************************//
//*********************** BMP TO MATRIX OF PIXEL *******************************//
//******************************************************************************//

Matrix bmp_to_matrix(SDL_Surface *image_surface){

  size_t width = (size_t)image_surface->w; //je recupère la largeur de l'image soit le nombre de pixel en largeur.

  size_t height = (size_t)image_surface->h; //je recupère la hauteur de l'image soit le nombre de pixel en hauteur.

  Matrix final;
  initMatrix(&final,height, width, false); // j'initialise une matrice de hauteur par largeur.

  for (size_t x = 0 ; x < width; x++) {
    for (size_t y = 0; y < height; y++) { //je parcour mes pixels un par un.

      Uint32 pixel = get_pixel(image_surface,x,y); // je recupère un pixel.
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); // je recupère ces valeur R G B.

      if (r == 255) //blanc
        final.data[y].data[x] = -1; //si c'est un pixel blanc je met ca valeur à 1.

      if (r == 0) //noir
        final.data[y].data[x] = 1;//si c'est un pixel noir je met ca valeur à 0.
    }
  }
  return final;
}

//******************************************************************************//
//******************** DETECTION OF CHARACTERS ********************************//
//******************************************************************************//

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//
