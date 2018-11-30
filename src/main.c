# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>

# include "pixel/pixel_operations.h"
# include "image_manipulation/to_binarize.h"
# include "image_manipulation/SDL_functions.h"
# include "decoupage/decoupage.h"
# include "matrix/matrix.h"


/*
	Installation : sudo apt-get install libsdl2-dev
	Compilation : Ubuntu 18.XX.XX
	gcc test.c -o exec $(sdl2-config --cflags --libs)
				  Windows
	gcc test.c -o exec -I SDL/include -L SDL/lib64 -lmingw64 -lSDL2main -lSDL2
*/

#define WIDTH 1920
#define HEIGHT 1080
#define XTIME 500

int main(){

	SDL_version nb;
	SDL_VERSION(&nb);
	SDL_Window *fenetre;
	SDL_Renderer *renderer;

	printf ("Hello, you're on SDL %d.%d\n", nb.major, nb.minor);


	//Lancement de SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitError("Initialisation SDL");

	//Creation de la fenetre
	fenetre = SDL_CreateWindow("image", //name of the window
								SDL_WINDOWPOS_CENTERED, //pos x
								SDL_WINDOWPOS_CENTERED, //pos y
								WIDTH, HEIGHT, //width & lenth
								0); //flag = window type

	if(fenetre == NULL) //verification en cas d'erreur
		SDL_ExitError("Creation de fenetre non abouti");

	//------ Creation Rendu ------//

	renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE/*flag*/);

	if(renderer == NULL)
		SDL_ExitError("Rendu non fait");

	//------ Importer l'image ------//

	SDL_Surface *image;
	SDL_Texture *texture;

	image = SDL_LoadBMP("Banque Image/words/wordsepita.bmp");

	if (image == NULL)
		SDL_ExitSupress("Image non crée", renderer, fenetre);

	//----------------- Application des fonctions sur l'image -----------------//

	otsu(image);
	printf("Binarize, done!\n");
	inverse(image);
	image = draw_lines(image);
	printf("Line Cuts\n");
	isolateLine(image);
	printf("Finish Treatment\n");


	//-------------------------------------------------------------------------//

	texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image); //on libere la surface qui ne sert plus a rien car la texture est crée

	if (texture == NULL)
		SDL_ExitSupress("Texture non crée", renderer, fenetre);

	SDL_Rect rectangle; //sera le rectangle contenant l'image

	if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0) //charge l'image en mémoire
		SDL_ExitSupress("Texture non chargée", renderer, fenetre);

	rectangle.x = (WIDTH - rectangle.w) / 2; //on centre le rectangle contenant l'image
	rectangle.y = (HEIGHT - rectangle.h) / 2;

	if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0) //copie la texture en mémoire sur le rendu
		SDL_ExitSupress("Impossible d'afficher la texture", renderer, fenetre);


	//------ Utilisation de la fenetre ------//

	SDL_RenderPresent(renderer); //Rafraichi la fenetre pour le rendu : SDL_RenderClear(renderer) pour effacer le rendu
	PressedKey();

	SDL_Delay(XTIME); //pause for x time in ms

	//------ FIN fenetre ------//

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();

	return 0;
}
