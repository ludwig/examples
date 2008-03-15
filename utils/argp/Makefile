CC = gcc

INCLUDES  =
LIBRARIES =
LIBS      =

CFLAGS = -Wall $(INCLUDES) $(LIBRARIES) $(LIBS)

TARGETS = \
	argp-ex1 \
	argp-ex2 \
	argp-ex3 \
	argp-ex4 \


all: $(TARGETS)

.c:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
