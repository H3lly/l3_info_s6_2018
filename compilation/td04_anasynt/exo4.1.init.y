/* exo4.1.y*/
/* faire d' abord l' exo3.11 i.e. EEE avec priorites */
/* produire la derivation droite pour tester l'effet des priorites choisies */
%{
#include  <stdio.h>
#include  <ctype.h>
extern int yylex();
int yyerror(char *s);
%}

%left PLUS MINUS
%left TIMES DIV
%left OR AND LT LE GT GE EQ DIFF
%right LPAR
%left RPAR
%right IF THEN
%right ELSE
%left SEMICOLON
%nonassoc NOT


%start S
%token IDENTIFIER AFF SEMICOLON IF THEN ELSE WHILE DO INTEGER PLUS MINUS TIMES DIV OR AND NOT LT LE GT GE EQ DIFF LPAR RPAR

%%


S:statement_list


statement_list:
	statement_list statement
	| statement
	;

statement:
	IDENTIFIER AFF expression SEMICOLON
	| IF expression THEN statement
	| IF expression THEN statement ELSE statement
	| WHILE expression DO statement
	;

expression:
	IDENTIFIER
	| INTEGER
	| expression PLUS expression
	| expression MINUS expression
	| expression TIMES expression
	| expression DIV expression
	| expression OR expression
	| expression AND expression
	| MINUS expression
	| NOT expression
	| expression LT expression
	| expression LE expression
	| expression GT expression
	| expression GE expression
	| expression EQ expression
	| expression DIFF expression
	| LPAR expression RPAR
	;

%%
#include "exo4.1.init.h"

int yyerror(char *s){
  fprintf( stderr, "*** ERROR: %s\n", s );
  return 0;
}

int main(int argn, char **argv){
  yyparse();
}
