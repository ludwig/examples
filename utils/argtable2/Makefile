CC = gcc

INCLUDES  =
LIBRARIES =
LIBS      = -largtable2

CFLAGS = -Wall

include Makefile.deps

all: $(TARGETS) $(OBJFILES)

argcustom: argcustom.c argxxx.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.c:
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJFILES)

.PHONY: clean
