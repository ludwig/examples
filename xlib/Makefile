CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L/usr/X11R6/lib
LIBS = -lX11

TARGETS = info first second

all: $(TARGETS)

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
