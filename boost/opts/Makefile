CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lboost_program_options -lboost_regex

TARGETS = \
	first \
	real \
	regex \
	custom_syntax \
	multiple_sources

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: clean
