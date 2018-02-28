CC = gcc 
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lm
FFLAG=-lfl
FLEX = flex
YACC = yacc
PREFIX = --file-prefix
BFLAGS= -d --report=all

.c.o: //passage de .c à .o (pour le code C)
	$(CC) -c -o $@ $(CFLAGS) $<
	//gcc -c -o nom_cible.o -g -Wall -std=c99 nom_fichier.c

.l.c:
	$(FLEX) -o $@ $<
	//flex -o nom_cible.c nom_fichier.l

.y.c: //passage du .y à .c en passant par la premiere regle
	$(YACC) $(PREFIX)= $* -d $<
	//yacc  --file-prefix=nom_fichier -d nom_fichier.y
	mv $*.tab.h $*.h
	mv $*.tab.c $*.c

clean:
	rm -f *.h lex.*.c *.o *~ \#*\#





/*

$(NOM):	a.out 
	mv a.out $(NOM).o

lex.yy.c: $(NOM).l
	$(FLEX) $(NOM).l

a.out: lex.yy.c
	$(CC) lex.yy.c $(FFLAG)

clean:
	rm -f lex.*.c *.o *~ \#*\#
*/
