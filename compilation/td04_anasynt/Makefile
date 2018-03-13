CC = gcc 
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lm
FFLAG=-lfl
FLEX = flex
YACC = yacc
PREFIX = --file-prefix
BFLAGS= -d --report=all


exo35: exo35-yacc.o exo35-lex.o
	$(CC) -o $@ $(LDFLAGS) $^ $(FFLAG)

.c.o:
	$(CC) -c -o $@ $(CFLAGS) $<

.l.c:
	$(FLEX) -o $@ $<

.y.c:
	$(YACC) $(PREFIX)=$* $(BFLAGS) $<
	mv $*.tab.h $*.h
	mv $*.tab.c $*.c


clean:
	rm -f *.h lex.*.c *.o *~ \#*\# exo34 exo33 exo35 *.output

