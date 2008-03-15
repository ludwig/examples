
TARGETS = \
	hello-c \
	hello-asm

default: $(TARGETS)

hello-c: hello.c
	gcc -Wall $< -o $@

hello-asm: hello.asm
	nasm -f elf $< -o hello-asm.o
	ld -s -o $@ hello-asm.o

clean:
	rm -f *.o

clean-all: clean
	rm -f $(TARGETS)

.PHONY: clean clean-all
