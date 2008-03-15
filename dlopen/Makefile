TARGETS = cos2 main1 main2

all: $(TARGETS)

cos2: cos2.c
	gcc -rdynamic $< -o $@ -ldl

hello.o: hello.cpp
	g++ -c $< -o $@

hello.so: hello.o
	g++ -shared $< -o $@

main1: main1.cpp hello.so
	g++ $^ -o $@ -ldl

triangle.o: triangle.cpp
	g++ -c $< -o $@

triangle.so: triangle.o
	g++ -shared $< -o $@

main2: main2.cpp triangle.so
	g++ $^ -o $@ -ldl

clean:
	rm -f *~ *.o *.so
	rm -f $(TARGETS)

.PHONY: clean