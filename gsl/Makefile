CC = gcc

#LIBS = -lgsl -lgslblas -lm
#LIBS = -lgsl -lcblas -lm
#LIBS = -lgsl -lcblas -latlas -lm
LIBS = -lgsl -lblas -latlas -lm

##############################################################################

PGMS = j0 lu eig_hil4 dgemm pulse_fft rosenbrock bsplines


default: $(PGMS)

%: %.c
	$(CC) $^ $(LIBS) -o $@ 

clean:
	rm -f $(PGMS)

.PHONY: clean
