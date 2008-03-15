CC = gcc
CFLAGS = -O3 -g -Wall
INCLUDES = -I/usr/include/python2.4 -I/usr/lib/python2.4/site-packages/numpy/core/include

all: ext_gridloop.so ext_gridloop_c.so ext_gridloop_f.so 

ext_gridloop_f.so: gridloop.f
	f2py -m ext_gridloop_f -c gridloop.f

gridloop_c.o: gridloop.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

gridloop3.o: gridloop3.c gridloop3.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

gridloop3_wrap.o: gridloop3_wrap.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


ext_gridloop_c.so: gridloop_c.o
	gcc -shared $^ -o $@

ext_gridloop.so: gridloop3.o gridloop3_wrap.o
	gcc -shared $^ -o $@

clean:
	rm -f *.so *.o *.pyc

.PHONY: clean
