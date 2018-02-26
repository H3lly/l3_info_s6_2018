CC = gcc
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lm
FFLAG=-lfl
FLEX = flex
BFLAGS=-d --report=all

NOM = exo1

#TD1/exo2.1

$(NOM):	a.out
	mv a.out $(NOM).o

lex.yy.c: $(NOM).l
	$(FLEX) $(NOM).l

a.out: lex.yy.c
	$(CC) lex.yy.c $(FFLAG)

clean:
	rm -f lex.*.c *.o *~ \#*\#
