##########################################################
CC=gcc
CFLAGS = -g -O2 -Wall

prog: afficheur
	-rm -rf *.o

afficheur: afficheur.o ensemble.o automate.o
	$(CC) afficheur.o ensemble.o automate.o $(CFLAGS) -o afficheur

afficheur.o: afficheur.c
	$(CC) -c afficheur.c $(CFLAGS)

ensemble.o:ensemble.c ensemble.h
	$(CC) -c ensemble.c $(CFLAGS)

automate.o:automate.c automate.h
	$(CC) -c automate.c $(CFLAGS)


clean:
	-rm -rf *.o afficheur
