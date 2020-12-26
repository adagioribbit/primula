.PHONY:	all

MKFDIR:=${CURDIR}/
SRCDIR:=${MKFDIR}src/
OBJDIR:=${SRCDIR}obj/

all: clean

make_obj_dir:
	mkdir -p ${OBJDIR}

list.o: make_obj_dir ${SRCDIR}List.h ${SRCDIR}List.cpp
	g++ -c -o ${OBJDIR}List.o ${SRCDIR}List.cpp -W -Wall

primelist.o: list.o ${SRCDIR}Primelist.cpp
	g++ -c -o ${OBJDIR}Primelist.o ${SRCDIR}Primelist.cpp

loop.o: list.o ${SRCDIR}Loop.h ${SRCDIR}Loop.cpp
	g++ -c -o ${OBJDIR}Loop.o ${SRCDIR}Loop.cpp

recurrence.o: primelist.o ${SRCDIR}Recurrence.h ${SRCDIR}Recurrence.cpp
	g++ -c -o ${OBJDIR}Recurrence.o ${SRCDIR}Recurrence.cpp

sieve.o: recurrence.o loop.o ${SRCDIR}Sieve.h ${SRCDIR}Sieve.cpp
	g++ -c -o ${OBJDIR}Sieve.o ${SRCDIR}Sieve.cpp 

main.o: sieve.o main.h main.cpp
	g++ -c -o ${OBJDIR}main.o main.cpp

sieve: main.o
	g++ -o sieve ${OBJDIR}List.o ${OBJDIR}Primelist.o ${OBJDIR}Loop.o ${OBJDIR}Recurrence.o ${OBJDIR}Sieve.o ${OBJDIR}main.o

clean: sieve
	rm -rf zero*
