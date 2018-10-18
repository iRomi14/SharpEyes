xornn: src/xor.o src/matrix.o src/vector.o
	gcc -o xornn src/xor.o src/matrix.o src/vector.o -lm

src/xor.o: src/xor.c
	gcc -o src/xor.o -c src/xor.c -Wall

src/matrix.o: src/matrix/matrix.c
	gcc -o src/matrix.o -c src/matrix/matrix.c -Wall

src/vector.o: src/matrix/vector.c
	gcc -o src/vector.o -c src/matrix/vector.c -Wall

clean:
	rm -rf */*.o
