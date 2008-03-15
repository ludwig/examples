CXX = g++
LIBS = -lginac
PGMS = hello hermite


default: $(PGMS)

%: %.cc
	$(CXX) $^ $(LIBS) -o $@ 

clean:
	rm -f $(PGMS)

.PHONY: clean
