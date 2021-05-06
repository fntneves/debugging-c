CC=gcc
CFLAGS=-g #-Wall

all: factorial print_person 

factorial: factorial.c

print_person: print_person.c person.o

person.o: person.c person.h

clean:
	@rm -rf factorial print_person *.dSYM *.o
