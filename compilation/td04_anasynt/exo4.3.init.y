%{
#include  <stdio.h>
#include  <ctype.h>
#include  <math.h>
#include  <stdlib.h>  
int yylex();
int yyerror(char *s);
  struct Tree{
    struct Tree *left;
    struct Tree *right;
    int tag;
    int s;
    int v;
  };
void evalTopBottom(struct Tree *);
void evalDownTop(struct Tree *);
  
%}
%union {
  struct Tree *tree;
}

%type<tree> N B

%%

S:	N {;};

N:	B {;}
	| N B {;}
        ;

B:	'0' {;}
	| '1' {;}
        ;

%%
  
int yylex(){
  return getchar();
}

int yyerror(char *s){
  fprintf( stderr, "*** ERROR: %s\n", s );
  return 0;
}

int main(int argn, char **argv){
  yyparse();
}
