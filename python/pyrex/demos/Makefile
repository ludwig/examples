all:
	python setup.py build_ext --inplace

test: all
	python run_primes.py 20

clean:
	@echo Cleaning demos
	rm -f *.c *.o *.so *~ core
	rm -rf build

.PHONY: clean
