CXX = g++
CXXFLAGS = -Wall
LIBS = -lboost_filesystem

TARGETS = \
	first \
	second \
	sample \
	listing0 \
	listing1 \
	listing5 \
	listing8 \
	listing9 \
	listing10 \
	file_size \
	simple_ls

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
