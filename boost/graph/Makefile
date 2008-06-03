CXX = g++
CXXFLAGS = -Wall
LIBS = -lboost_graph


TARGETS = \
	file_dependencies \
	kevin-bacon \


default: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f *.o

purge: clean
	rm -f $(TARGETS)

.PHONY: clean purge
