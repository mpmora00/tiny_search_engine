# Makefile for Common library
#   Builds and compiles all these utility functions into a library.
#
# Maria Paula Mora, April 2020

# object files, and the target library
LDIR = ../libcs50
OBJS = pagedir.o word.o index.o
LIB = common.a

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
word.o: word.h
index.o: index.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o