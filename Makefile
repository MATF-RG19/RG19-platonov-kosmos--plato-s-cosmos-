PROGRAM   = platonovKosmos
CC        = gcc
CFLAGS    = -g -Wall
LDLIBS    = -lGL -lGLU -lglut -lm


$(PROGRAM): main.o objekti.o image.o
	$(CC) $(CFLAGS) -o $(PROGRAM) *.o $(LDLIBS)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c $(LDLIBS)

objekti.o: src/objekti.c headers/objekti.h
	$(CC) $(CFLAGS) -c src/objekti.c $(LDLIBS)

image.o: src/image.c
	$(CC) $(CFLAGS) -c src/image.c $(LDLIBS)

.PHONY: clean dist run

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

run:
	./$(PROGRAM)