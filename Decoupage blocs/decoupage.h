#ifndef DECOUPAGE_H
#define DECOUPAGE_H

# include "../matrix/matrix.h"

void rlsa ( int *tabBin[] , size_t seuil );

void rlsaALL ( int **tabBin, int seuil );

Matrix bmp_to_matrix(SDL_Surface *image_surface);

Matrix matrix_ligne(Matrix enter_matrix);

#endif
