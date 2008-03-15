
HDF5_HOME=$(HOME)/opt/hdf5/1.6.6

CC = gcc

CFLAGS  = -Wall -g
CFLAGS += -I$(HDF5_HOME)/include

LDFLAGS  = -L$(HDF5_HOME)/lib
LDFLAGS += -Wl,--rpath -Wl,$(HDF5_HOME)/lib

LIBS = -lhdf5

##############################################################################

TARGETS = \
	write.exe \
	group.exe

all: $(TARGETS)

%.exe: %.o
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o
	rm -f $(TARGETS)

.PHONY: clean
