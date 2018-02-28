CC = gcc 
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lm
FFLAG=-lfl
FLEX = flex
YACC = yacc
PREFIX = --file-prefix
BFLAGS= -d --report=all

.c.o: 
	$(CC) -c -o $@ $(CFLAGS) $<


.l.c:
	$(FLEX) -o $@ $<


.y.c: 
	$(YACC) $(PREFIX)=$* -d $<
	mv $*.tab.h $*.h
	mv $*.tab.c $*.c

clean:
	rm -f *.h lex.*.c *.o *~ \#*\#

