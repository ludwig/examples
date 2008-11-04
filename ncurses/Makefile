
CC = gcc
CFLAGS = -Wall
LIBS = -lncurses

TARGETS = \
    cls \
    add1 add2 add3 \
    yoda typewriter \
    string1 \
    goodbye

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean

