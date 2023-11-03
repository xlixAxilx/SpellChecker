main: spell.o spell_lib.o
		gcc -o main spell.o spell_lib.o

spell.o: spell.h spell.c
		gcc -c spell.c

spell_lib.o: spell_lib.h spell_lib.c
		gcc -c spell_lib.c

clean:
		rm *.o

all: main clean
