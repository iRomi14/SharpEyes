CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O1 -I/usr/include/SDL2 -D_REENTRANT `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
LDLIBS = -lm -L/usr/lib/x86_64-linux-gnu -lSDL2

SRC_NN = src/neural_net/nn.c src/matrix/matrix.c src/matrix/vector.c
SRC_SDL = src/pixel/pixel_operations.c src/image_manipulation/to_binarize.c src/image_manipulation/SDL_functions.c src/decoupage/decoupage.c

# Main
SRC = src/image_manipulation/main.c src/image_manipulation/open_image.c src/image_manipulation/GTK_functions.c ${SRC_NN} ${SRC_SDL}
OBJ = ${SRC:.c=.o}

main: ${OBJ}
	$(CC) -o main ${OBJ} ${CFLAGS} ${LDLIBS}

SRC_OLD = src/main.c ${SRC_NN} ${SRC_SDL}
OBJ_OLD = ${SRC_OLD:.c=.o}

main_old: ${OBJ_OLD}
	$(CC) -o main_old ${OBJ_OLD} ${CFLAGS} ${LDLIBS}

SRC_TRAIN = src/train.c ${SRC_NN} ${SRC_SDL}
OBJ_TRAIN = ${SRC_TRAIN:.c=.o}

train: ${OBJ_TRAIN}
	$(CC) -o train ${OBJ_TRAIN} ${CFLAGS} ${LDLIBS}

SRC_OCR = src/train.c ${SRC_NN} ${SRC_SDL}
OBJ_OCR = ${SRC_OCR:.c=.o}

ocrnn: ${OBJ_OCR}
	$(CC) -o train ${OBJ_OCR} ${CFLAGS} ${LDLIBS}

SRC_XOR = src/xor.c ${SRC_NN} ${SRC_SDL}
OBJ_XOR = ${SRC_XOR:.c=.o}

xornn: ${OBJ_XOR}
		$(CC) -o xornn ${OBJ_XOR} ${CFLAGS} ${LDLIBS}

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_OLD}
	${RM} ${OBJ_TRAIN}
	${RM} ${OBJ_XOR}
	${RM} ${OBJ_OCR}
