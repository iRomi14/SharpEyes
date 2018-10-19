CC=gcc

CFLAGS= -Wall -Wextra -Werror -std=c99 -O1
LDFLAGS= -lm

xornn: src/xor.o src/matrix.o src/vector.o
	CC -o xornn src/xor.o src/matrix.o src/vector.o LDFLAGS

src/xor.o: src/xor.c
	CC -o src/xor.o -c src/xor.c CFLAGS

src/matrix.o: src/matrix/matrix.c
	CC -o src/matrix.o -c src/matrix/matrix.c CFLAGS

src/vector.o: src/matrix/vector.c
	CC -o src/vector.o -c src/matrix/vector.c CFLAGS

clean:
	rm -rf */*.o
