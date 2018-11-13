CC=gcc

CFLAGS= -Wall -Wextra -Werror -std=c99 -O1
MATHFLAGS= -lm
SDLFLAGS = -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/x86_64-linux-gnu -lSDL2


main: src/main.o src/pixel_operations.o src/to_binarize.o src/decoupage.o src/matrix.o src/vector.o
	$(CC) -o main src/main.o src/pixel_operations.o src/to_binarize.o src/decoupage.o src/matrix.o src/vector.o  $(SDLFLAGS) $(MATHFLAGS)

src/main.o: src/main.c
	$(CC) -o src/main.o -c src/main.c $(CFLAGS) $(SDLFLAGS)

src/pixel_operations.o: src/pixel/pixel_operations.c
	$(CC) -o src/pixel_operations.o -c src/pixel/pixel_operations.c $(CFLAGS) $(SDLFLAGS)

src/to_binarize.o: src/image_manipulation/to_binarize.c
	$(CC) -o src/to_binarize.o -c src/image_manipulation/to_binarize.c $(CFLAGS) $(SDLFLAGS)

src/decoupage.o: src/decoupage/decoupage.c src/matrix.o src/vector.o
	$(CC) -o src/decoupage.o src/matrix.o src/vector.o -c src/decoupage/decoupage.c $(CFLAGS) $(SDLFLAGS) $(MATHFLAGS)

xornn: src/xor.o src/matrix.o src/vector.o src/nn.o
	$(CC) -o xornn src/xor.o src/matrix.o src/vector.o src/nn.o $(MATHFLAGS)

src/xor.o: src/xor.c
	$(CC) -o src/xor.o -c src/xor.c $(CFLAGS)

src/nn.o: src/neural_net/nn.c
	$(CC) -o src/nn.o -c src/neural_net/nn.c $(CFLAGS)

src/matrix.o: src/matrix/matrix.c
	$(CC) -o src/matrix.o -c src/matrix/matrix.c $(CFLAGS)

src/vector.o: src/matrix/vector.c
	$(CC) -o src/vector.o -c src/matrix/vector.c $(CFLAGS)

clean:
	rm -rf */*.o
