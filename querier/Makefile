# Makefile for crawler.c
#
# Maria Paula Mora - April 2020

LDIR=../libcs50
CDIR=../common

PROG=testing.sh

PROG1=querier
OBJS=querier.o iterate.o pair.o
LIB=$(CDIR)/common.a $(LDIR)/libcs50.a 

TESTPROG=fuzzquery
TESTOBJ=fuzzquery.o
TESTLIB = $(LDIR)/libcs50.a
TESTFLAGS = -Wall -pedantic -std=gnu11 -ggdb $(FLAGS) -I $(LDIR)

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR) -I $(CDIR)
CC = gcc
MAKE = make

$(PROG1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

$(TESTPROG): $(TESTOBJ)
	$(CC) $(TESTFLAGS) $^ $(TESTLIB) -o $@

# Dependencies: object files depend on header files
querier.o: iterate.h pair.h

.PHONY:  clean test all

all:
	$(MAKE) $(PROG1)
	$(MAKE) $(TESTPROG)

test: $(PROG)
	  bash $(PROG)

clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks
	rm -f $(PROG1)
	rm -f inputs/fuzzquery_input*

