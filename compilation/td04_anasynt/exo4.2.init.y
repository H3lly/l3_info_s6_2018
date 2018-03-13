%{
#include  <stdio.h>
#include  <ctype.h>
#include  <math.h>
#include  <stdlib.h>
#include  <string.h>
extern int yylex();
int yyerror(char *s);
 
enum Tag {ET, OU, NON, IMPLIC, EQUIV, CONST, VARIABLE};
 
struct Tree{
  struct Tree *left;
  struct Tree *right;
  enum Tag tag;
  int value;
  };
  
struct Tree *createTree();
void printTree(struct Tree *tree);
 
%}

%start start
%token NOT AND OR IMP EQ TRUE FALSE VAR
%%

start: expr ;

expr:	 expr AND expr
	| expr OR expr 
	| expr IMP expr 
	| expr EQ expr 
	| NOT expr 
	| VAR
	| TRUE
	| FALSE
	|'(' expr ')'
	;

%%
#include "lex.yy.c"

int yyerror(char *s){
  fprintf( stderr, "*** ERROR: %s\n", s );
  return 0;
}

struct Tree *createTree()
    {return (struct Tree*) malloc (sizeof(struct Tree));
    }

int main(int argn, char **argv){
  yyparse();
}
