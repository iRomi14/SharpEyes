

EXEC=window


all:	$(EXEC)

window:	main.o
		gcc -o window main.o	

main.o:	main.c
		gcc -o main.o -c main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:	
		rm -rf *.o

mrproper:	clean
			rm -rf $(EXEC)