BISON = bison
BFLAGS= -d --report=all
EXO = exo4.1.init

exo4.1:
	$(BISON) $(EXO).y $(BFLAGS)
	mv $(EXO).output $(EXO)
	rm *.h *.c


clean:
	rm -f *.h *.c $(EXO)
