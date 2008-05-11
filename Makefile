CC = mpicc.mpich-shmem
CCFLAGS = -Wall
LDFLAGS = -lm
SCRIPT = run_fox.sh
PWD = $(shell pwd)

PROGRAMS = fox

all: $(PROGRAMS)

clean: rmout
	-rm -rv $(PROGRAMS) *.o
rmout:
	-rm $(SCRIPT).{o,e}*

matrix.o: matrix.c matrix.h
fox.o: fox.c
fox: fox.o matrix.o
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $@ 

matrix_test.o: matrix_test.c matrix.c matrix.h
matrix_test: matrix_test.o matrix.o
	$(CC) $(CCFLAGS) $^ -o $@ 


.PHONY: all clean rmout

