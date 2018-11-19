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
