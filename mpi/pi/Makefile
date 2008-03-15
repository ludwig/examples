CC = mpicc

pi: pi.c
	$(CC) $^ -o $@

run:
	bsub -W01:00 < run.sh

stat:
	@bstat -me

clean:
	rm -f pi pi.o

.PHONY: clean
