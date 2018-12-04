CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O3 -I/usr/include/SDL2 -D_REENTRANT `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
LDLIBS = -lm -L/usr/lib/x86_64-linux-gnu -lSDL2

SRC_NN = src/neural_net/nn.c src/matrix/matrix.c src/matrix/vector.c
SRC_SDL = src/pixel/pixel_operations.c src/image_manipulation/to_binarize.c src/image_manipulation/smooth.c src/image_manipulation/SDL_functions.c src/decoupage/decoupage.c  src/decoupage/reconstruct.c src/image_manipulation/rotate.c

all: main

# Main
SRC = src/main.c src/image_manipulation/open_image.c src/image_manipulation/GTK_functions.c ${SRC_NN} ${SRC_SDL}
OBJ = ${SRC:.c=.o}

main: ${OBJ}
	$(CC) -o main ${OBJ} ${CFLAGS} ${LDLIBS}

SRC_TRAIN = src/train.c ${SRC_NN} ${SRC_SDL}
OBJ_TRAIN = ${SRC_TRAIN:.c=.o}

train: ${OBJ_TRAIN}
	$(CC) -o train ${OBJ_TRAIN} ${CFLAGS} ${LDLIBS}


SRC_XOR = src/xor.c ${SRC_NN} ${SRC_SDL}
OBJ_XOR = ${SRC_XOR:.c=.o}

xornn: ${OBJ_XOR}
		$(CC) -o xornn ${OBJ_XOR} ${CFLAGS} ${LDLIBS}

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TRAIN}
	${RM} ${OBJ_XOR}
