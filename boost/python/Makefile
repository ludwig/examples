# gcc -shared -Wl,-soname,hello -I/usr/include `python-config --includes` `python-config --libs` -lboost_python -o hello.so hello.cpp

CXX = g++
LDFLAGS = -shared -Wl,-soname,hello
INCLUDES = `python-config --includes`
LIBS  = `python-config --libs`
LIBS += -lboost_python

CXXFLAGS = -Wall
CXXFLAGS += $(LDFLAGS)
CXXFLAGS += $(INCLUDES)
CXXFLAGS += $(LIBS)

hello.so: hello.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f hello.so

.PHONY: clean
