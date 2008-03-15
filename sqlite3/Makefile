CC = gcc
CFLAGS = -Wall -g -O3
LIBS = -lsqlite3


TARGETS = myquery


all: $(TARGETS)

.c:
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
