#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "matrix/matrix.h"

int main(){
  srand (time(NULL));

  static unsigned int EPOCHS = 1000;

  /*
   * X = [[0,0],
   *      [0,1],
   *      [1,0],
   *      [1,1]]
   */
  Matrix X = initMatrix(4, 2, false);
  X.data[0].data[0] = 0.0;
  X.data[0].data[1] = 0.0;
  X.data[1].data[0] = 1.0;
  X.data[1].data[1] = 0.0;
  X.data[2].data[0] = 0.0;
  X.data[2].data[1] = 1.0;
  X.data[3].data[0] = 1.0;
  X.data[3].data[1] = 1.0;

  /*
   * y = xor(X) = [[0],
   *               [1],
   *               [1],
   *               [0]]
   */
  Matrix y = initMatrix(4, 1, false);
  y.data[0].data[0] = 0.0;
  y.data[1].data[0] = 1.0;
  y.data[2].data[0] = 1.0;
  y.data[3].data[0] = 0.0;

  
  Matrix w0 = initMatrix(2, 4, true);
  Matrix w1 = initMatrix(4, 1, true);

  Matrix l1, l2;
  Matrix l2_error, l1_error;
  Matrix l2_delta, l1_delta;

  int j;
  for(j = 0; j < EPOCHS; j++){
    l1 = sigmoidMatrix(dotMatrix(X, w0), false);
    l2 = sigmoidMatrix(dotMatrix(l1, w1), false);

    l2_error = addMatrix(y, scalarMatrix(l2, -1.0)); // y - l2

    //printMatrix(l2);
    //printf("-----------------\n");
    

    if (j% 100 == 0)
      printf ("Cost at %d epochs : %lf\n", j, meanMatrix(l2_error));


    l2_delta = multMatrix(l2_error,sigmoidMatrix(l2,true));

    l1_error = dotMatrix(l2_delta, transpose(w1));
    l1_delta = multMatrix(l1_error,sigmoidMatrix(l1,true));

    w1 = addMatrix(w1, dotMatrix(transpose(l1), l2_delta));
    w0 = addMatrix(w0, dotMatrix(transpose(X), l1_delta));

  }

  printf("---------Output---------\n");

  Matrix x = initMatrix(1, 2, false);
  x.data[0].data[0] = 0.0;
  x.data[0].data[1] = 0.0;
  printf("Xor(0, 0) = %lf ( expected : %lf )\n", sigmoidMatrix(dotMatrix(sigmoidMatrix(dotMatrix(x, w0), false), w1), false).data[0].data[0], y.data[0].data[0]);

  //printMatrix(x);

  x.data[0].data[0] = 1.0;
  x.data[0].data[1] = 0.0;
  printf("Xor(1, 0) = %lf ( expected : %lf )\n", sigmoidMatrix(dotMatrix(sigmoidMatrix(dotMatrix(x, w0), false), w1), false).data[0].data[0], y.data[1].data[0]);

  x.data[0].data[0] = 0.0;
  x.data[0].data[1] = 1.0;
  printf("Xor(0, 1) = %lf ( expected : %lf )\n", sigmoidMatrix(dotMatrix(sigmoidMatrix(dotMatrix(x, w0), false), w1), false).data[0].data[0], y.data[2].data[0]);

  x.data[0].data[0] = 1.0;
  x.data[0].data[1] = 1.0;
  printf("Xor(1, 1) = %lf ( expected : %lf )\n", sigmoidMatrix(dotMatrix(sigmoidMatrix(dotMatrix(x, w0), false), w1), false).data[0].data[0], y.data[3].data[0]);

  return 0;
}
