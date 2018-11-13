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

}*/

//******************************************************************************//
//********************** DETECTION OF WHITE LINES ******************************//
//******************************************************************************//

SDL_Surface* lines(SDL_Surface* img)
{
	int w = img -> w;        // Largeur de l'image.(=nombre de pixel en largeur)
	int h = img -> h;        // Hauteur de l'image.(=nombre de pixel en hauteur)
	Uint8 r , g , b;         // Uint8 = nombres signés de 0 à 255 inclue.
  int line = 0;            // Marque la présence de la ligne, ici (au début) il n'y en a pas. Nous sert de booléen pour la suite.
	int black = 0;           // Marque la présence du pixel noir.
	int begin;               // Ligne du début de la présence du pixel noir.
	int end;                 // Ligne de la fin de la présence du pixel noir.
	SDL_Surface* ligne_img = img;

	for (int y = 0; y < h; y++){        // Parcours des lignes.
		for (int x = 0; x < w; x++){      // Parcours des colonnes.
			Uint32 pixel = get_pixel(ligne_img, x, y);
			SDL_GetRGB(pixel, ligne_img->format, &r, &g, &b);

			if (!r){      // Si le pixel est noir.
				black = 1;  // Black = True.
				break;      // Sortir de la boucle.
			}
		}

		if (black && !line){  // Si la ligne a des pixels noirs.
			line = 1;           // Line = True.
			begin = y;          // On conserve l'index de la ligne.

			for (int x = 0; x < w; x++){
        Uint32 pixel = get_pixel(ligne_img, x, y);
				int pos = y - 1;   // Précédente ligne.
				if (pos >= 0){     // Vérifie si la hauteure de la ligne précédente est positive.
					pixel = SDL_MapRGB(ligne_img->format, 255, 0, 0);  //Change la couleur de la ligne précédente. (ROUGE)
	        put_pixel(ligne_img, x, pos, pixel);
				}
			}
		}

		if (!black && line){    // Si la ligne précédente a des pixels noirs mais maintenant c'est blanc.
			line = 0;             // Line = False
			end = y;              // Conserve l'index de la fin de ligne.

      for (int x = 0; x < w; x++){
	      Uint32 pixel = get_pixel(ligne_img, x, y);
				int pos = y + 1;      // Ligne suivante
        if (pos < h){         // Verifie si la prochaine ligne peux être atteinte.
          pixel = SDL_MapRGB(ligne_img->format, 255, 0, 0); //(ROUGE)
          put_pixel(ligne_img, x, pos, pixel);
        }
      }
			ligne_img = columns(ligne_img, begin, end);
		}
		black = 0;    //Black = False.
	}
	return ligne_img;
}


SDL_Surface* columns(SDL_Surface* img, int b_line, int e_line)
{
	int w = img -> w;            // Largeur de l'image.(=nombre de pixel en largeur)
	Uint8 r , g , b;             // Uint8 = nombres signés de 0 à 255 inclue.
	int end_c = 0;               // Marque la fin d'une rencontre avec une lettre.(= un caractère)
	int black = 0;               // Trace la première ligne du caractère.
	int e_black = 0;             // trace la dernière ligne du caractère.
	int white = 0;               // SI toutes les colonnes sont blanches.
	SDL_Surface* c_img = img;

	for(int x = 0; x < w; x++){               // Parcours des colonnes
		white = 1;
		for(int y = b_line; y < e_line; y++){   // Parcours des lignes.

			Uint32 pixel = get_pixel(c_img, x, y);
      SDL_GetRGB(pixel, c_img->format, &r, &g, &b);

			if (!r && !end_c)// Première rencontre avec un caractère.
			{
				end_c = 1;  // Le début du caractère est là.
				black = 1;  // On doit tracer la première ligne.
				break;
			}

			if(!r){     // Si le caractère n'est pas encore fini.
				white = 0;
				break;
			}
			// had encountered a character but now it's its end with y,
			// also verify if this column has not black pixel with the white satement
			if (b == 255 && end_c && white && y == e_line - 1)
			{
				end_c = 0;  // La fin du caractère est là.
				e_black = 1;// On doit tracer la dernière ligne.
				break;
			}
		}

		if (end_c && black){  // Coloré la précédente colonne du caractère.
			black = 0;
			for (int y = b_line; y < e_line; y++){
				if (x - 1 > 0){//previous column
          Uint32 pixel = SDL_MapRGB(c_img->format, 0, 0, 255); //Change la couleur de la colonne.(BLEU)
          put_pixel(c_img, x - 1, y, pixel);
        }
			}
		}

		if (!end_c && e_black){   // color the next column of the caracter
			e_black = 0;            // last line was traced, so make back the statement
			for (int y = b_line; y < e_line; y++){
        Uint32 pixel = SDL_MapRGB(c_img->format, 0, 0, 255); //Change la couleur de la colonne. (BLEU)
        put_pixel(c_img, x, y, pixel);
			}
		}
  }
	return c_img;
}

//******************************************************************************//
//******************** DETECTION OF CHARACTERS ********************************//
//******************************************************************************//

//******************************************************************************//
//******************** ADJUSTEMENT OF CHARACTERS *******************************//
//******************************************************************************//
