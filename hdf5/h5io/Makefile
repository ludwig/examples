#include ../../build/variables

HDF5_HOME = $(HOME)/opt/hdf5/1.6.5-serial
HDF5_INCLUDES = -I$(HDF5_HOME)/include


CC = gcc
OPTIM = -O2
FLAGS = -Wall -g

AR = /usr/bin/ar ruvs
RANLIB = /usr/bin/ranlib

#LIB = ../../lib/libh5io.a
LIB = libh5io.a

INCLUDES += $(HDF5_INCLUDES)
CFLAGS = $(OPTIM) $(FLAGS) $(INCLUDES)

SRC = \
	h5array.c \
	h5attr.c \
	h5dset.c \
	h5file.c \
	h5group.c \
	split.c \

OBJ = $(SRC:.c=.o)

.SUFFIXES: .o .c

$(LIB): $(OBJ)
	$(AR) $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $<

tests:
	cd t && make tests

clean:
	rm -f *.o
	cd t && make clean

-include Makefile.dep
depend:
	$(CC) -MM $(SRC) > Makefile.dep

.PHONY: clean depend
