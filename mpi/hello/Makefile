CC = mpicc

mpihello: mpihello.c
	$(CC) $^ -o $@

run:
	bsub -W01:00 < run.sh

stat:
	@bstat me

clean:
	rm -f mpihello mpihello.o

.PHONY: clean
