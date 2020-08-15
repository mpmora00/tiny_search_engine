# Makefile for index.c
#
# Maria Paula Mora - May 2020

LDIR=../libcs50
CDIR=../common

PROG = testing.sh
PROG1= indexer
PROG2 = indextest
OBJ1 = indexer.o 
OBJ2 = indextest.o 
LIB = $(CDIR)/common.a $(LDIR)/libcs50.a

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(CDIR) -I $(LDIR)
CC = gcc
MAKE = make

$(PROG1): $(OBJ1)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

$(PROG2): $(OBJ2)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

.PHONY:  clean test all

all: indextest indexer
	
test: $(PROG)
	  bash $(PROG)

clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks
	rm -f $(PROG1)
	rm -fr result