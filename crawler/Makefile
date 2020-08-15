# Makefile for crawler.c
#
# Maria Paula Mora - April 2020

LDIR=../libcs50
CDIR=../common

PROG1= crawler
PROG = testing.sh
OBJS = crawler.o 
LIB = $(LDIR)/libcs50.a $(CDIR)/common.a

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR) -I $(CDIR)
CC = gcc
MAKE = make

$(PROG1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

.PHONY:  clean test

test: $(PROG)
	  bash $(PROG)

clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks
	rm -fr data 
	rm -fr data*
	rm -fr wiki*