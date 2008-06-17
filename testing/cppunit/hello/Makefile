CPPUNIT_PREFIX = /usr

CXX = g++
FLAGS += -Wall
LIBS += -lcppunit -ldl
LDFLAGS += -L$(CPPUNIT_PREFIX)/lib
LDFLAGS += $(LIBS)

default: hello

hello.o: hello.cpp
	$(CXX) $(FLAGS) -c $^ -o $@

hello: hello.o
	$(CXX) $(FLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f hello
	rm -f hello.o

.PHONY: clean
	
