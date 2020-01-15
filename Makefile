PROGRAM   = main
CC        = gcc
CFLAGS    = -g -Wall
LDLIBS    = -lGL -lGLU -lglut -lm

main: main.o stars.o
	$(CC) $(CFLAGS) -o main *.o $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LDLIBS)
	
stars.o:
	$(CC) $(CFLAGS) -c stars.c $(LDLIBS)


.PHONY: clean dist run

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

run:
	./$(PROGRAM)

