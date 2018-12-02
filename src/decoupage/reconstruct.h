#ifndef RECONSTRUCT_H
#define RECONSTRUCT_H

# include <SDL.h>
# include "../neural_net/nn.h"
# include "../pixel/pixel_operations.h"
# include "../image_manipulation/SDL_functions.h"

struct text {
  char *data;
  size_t size;
  size_t idx;
} Final_Text;

NN ocrNet;

void bmp_to_vector(Vector *dst, SDL_Surface *image_surface);
void initText();
void addChar(char c);
void OCR_recon(SDL_Surface *image_surface);


#endif
