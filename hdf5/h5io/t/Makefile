#include ../../../build/variables
#H5IO_LIB = ../../../lib/libh5io.a

HDF5_HOME = $(HOME)/opt/hdf5/1.6.5-serial
HDF5_INCLUDES = -I$(HDF5_HOME)/include
H5IO_LIB = ../libh5io.a


INCLUDES += $(HDF5_INCLUDES)
LIBS += $(CIGMA_LIBS)
LIBS += -lh5io
LIBS += -lhdf5
#LDFLAGS += -L../../../lib
LDFLAGS += -L..
LDFLAGS += $(HDF5_LDFLAGS)

OPTIM += -fno-strict-aliasing
FLAGS += $(INCLUDES)

CFLAGS = $(OPTIM) $(FLAGS) $(LDFLAGS)

TESTS = \
	test_h5file \
	test_h5group \
	test_h5dset \

tests: $(TESTS)

test_%: test_%.c $(H5IO_LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f test_io.h5
	rm -f $(TESTS)

.PHONY: clean
