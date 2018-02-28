%{
  #include <stdio.h>
%}


%start start
%token NUMBER

%%
start: E;
E:	 E '\n'  {printf("%d\n", $1);}
	|E '+' T {$$ = $1 + $3;}
	|E '-' T {$$ = $1 - $3;}
	|T	 {$$ = $1;}

T:	 T '*' F {$$ = $1 * $3;}
	|T '/' F {$$ = $1 / $3;}
	|F	 {$$ = $1;}

F:	NUMBER	   {$$ = $1;}
	|'(' E ')' {$$ = $2;}
;
%%

int main(){
	yyparse();
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*** ERROR: %s\n", s);
	return 1;
}
