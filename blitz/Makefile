include Makefile.deps

CXX = g++
CXXFLAGS = -Wall -g -O3
LIBS = -lblitz -lm

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
