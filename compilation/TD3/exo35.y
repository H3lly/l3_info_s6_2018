%{
  #include <stdio.h>
  %}

%start start

%%
start: E '\n';  {printf("%d\n", $1);}
E:     E '+' T  {$$ = $1 + $3;}
     | E '-' T  {$$ = $1 - $3;}
     | T        {$$ = $1;}
;
T:     T '*' F  {$$ = $1 * $3;}
     | T '/' F  {$$ = $1 / $3;}
     | F        {$$ = $1;}
;
F:     NUMBER   {$$ = $1;}
     | E        {$$ = $1;}
%%

int yylex(){
  return getchar();
}

int main(){
  yyparse();
  return 0;
}

int yyerror(char *s){
  fprintf(stderr, "*** ERROR: %s\n", s);
  return 0;
}
