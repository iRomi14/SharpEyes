CC=gcc

CFLAGS= -Wall -Wextra -Werror -std=c99 -O1
MATHFLAGS= -lm
SDLFLAGS = -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/x86_64-linux-gnu -lSDL2


main: src/main.o src/pixel_operations.o
	$(CC) -o main src/main.o src/pixel_operations.o $(SDLFLAGS)

src/main.o: src/main.c
	$(CC) -o src/main.o -c src/main.c $(CFLAGS) $(SDLFLAGS)

src/pixel_operations.o: src/pixel/pixel_operations.c
	$(CC) -o src/pixel_operations.o -c src/pixel/pixel_operations.c $(CFLAGS) $(SDLFLAGS)


xornn: src/xor.o src/matrix.o src/vector.o
	$(CC) -o xornn src/xor.o src/matrix.o src/vector.o $(MATHFLAGS)

src/xor.o: src/xor.c
	$(CC) -o src/xor.o -c src/xor.c $(CFLAGS)

src/matrix.o: src/matrix/matrix.c
	$(CC) -o src/matrix.o -c src/matrix/matrix.c $(CFLAGS)

src/vector.o: src/matrix/vector.c
	$(CC) -o src/vector.o -c src/matrix/vector.c $(CFLAGS)

clean:
	rm -rf */*.o
