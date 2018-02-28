%{
  #include <stdio.h>
%}


%start start

%%
start: S;
S:	 'a' S 'b'
	|'a'   'b'
	|
;
%%

int yylex(){
	return getchar();
}

int main(){
	if(!yyparse())
		printf("\nLangage reconnu.\n");
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*** ERROR: %s\n", s);
	return 1;
}
