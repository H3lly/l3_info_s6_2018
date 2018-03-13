/* td4.1.l*/
%{
/*
pas de #include "exo4.1.tab.h"
*/
%}
/* NB: il manque IF THEN ELSE WHILE DO */
%option nounput noinput
%%
[A-Za-z][A-Za-z0-9]* {return IDENTIFIER;}
0|[1-9][0-9]*        {return INTEGER;}
";"                  {return SEMICOLON;}
"+"                  {return PLUS;}
"-"                  {return MINUS;}
"*"                  {return TIMES;}
"/"                  {return DIV;}      
"||"                 {return OR;} 
"&&"                 {return AND;}
"!"                  {return NOT;}
"<"                  {return LT;}
"<="                 {return LE;}
">"                  {return GT;}
">="                 {return GE;}
"="                  {return EQ;}
"!="                 {return DIFF;}
"("                  {return LPAR;}
")"                  {return RPAR;}
[ \t\n]           {;}
.              {return *yytext;}

%%

int yywrap(){
   return 1;
}
