CC = gcc
CFLAGS = -Wall

TARGETS = tortoise1 tortoise2 tortoise3 tortoise4 tortoise5

all: $(TARGETS)

tortoise1: tortoise1.c
	$(CC) $< -o $@ -lX11 -lm

tortoise2: tortoise2.c
	$(CC) $< -o $@ -lX11 -lm -lguile

tortoise3: tortoise3.c
	$(CC) $< -o $@ -lX11 -lm -lguile

tortoise4: tortoise4.c
	$(CC) $< -o $@ -lX11 -lm -lguile

tortoise5: tortoise5.c
	$(CC) $< -o $@ -lX11 -lm -lguile

clean:
	rm -f *~
	rm -f tortoise?

.PHONY: clean
