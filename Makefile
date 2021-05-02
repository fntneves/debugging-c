CC=gcc
CFLAGS=-g #-Wall

all: factorial free_example struct_segfault pipe_deadlock

pipe_deadlock: pipe_deadlock.c

factorial: factorial.c

free_example: free_example.c person.o

struct_segfault: struct_segfault.c person.o

person.o: person.c person.h

clean:
	@rm -rf factorial free_example struct_segfault pipe_deadlock *.dSYM *.o
