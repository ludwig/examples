
TARGETS = \
	init \
	do-me \
	add \
	average \


all: $(TARGETS)


init: init.c
	gcc -o init -Wall `lua-config --include --libs` init.c

do-me: do-me.c
	gcc -o do-me -Wall `lua-config --include --libs` do-me.c

add: add.c
	gcc -o add -Wall `lua-config --include --libs` add.c

average: average.c
	gcc -o average -Wall `lua-config --include --libs` average.c

clean:
	rm -f $(TARGETS)

.PHONY: clean
