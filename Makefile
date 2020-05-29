PROGRAM = game
CC      = gcc
CFLAGS  = -g -ansi -Wall -I/usr/X11R6/include -I/usr/pkg/include -std=c99
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o image.o sto.o loptice.o cunjevi.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(PROGRAM)  main.o image.o sto.o loptice.o cunjevi.o $(LDLIBS)

image.o: image.c
	$(CC) $(LDFLAGS) $(CFLAGS) -c image.c

sto.o: sto.c
	$(CC) $(LDFLAGS) $(CFLAGS) -c sto.c

loptice.o: loptice.c
	$(CC) $(LDFLAGS) $(CFLAGS) -c loptice.c

cunjevi.o: cunjevi.c
	$(CC) $(LDFLAGS) $(CFLAGS) -c cunjevi.c

main.o: main.c 
	$(CC) $(LDFLAGS) $(CFLAGS) -c main.c  

.PHONY: clean

clean:
	-rm *.o $(PROGRAM)
