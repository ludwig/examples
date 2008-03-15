CXX = g++
LIBS = -lginac -lcln
PGMS = hello hermite


default: $(PGMS)

%: %.cc
	$(CXX) $^ $(LIBS) -o $@ 

clean:
	rm -f $(PGMS)

.PHONY: clean
