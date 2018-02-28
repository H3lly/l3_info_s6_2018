%{
	#include <stdio.h>
	const int NUMBER = 0;
%}

entier [0-9]+

%option noyywrap

%%
{entier} {return NUMBER;}
. {return (int)yytext[0];}
%%



int main(){
	int c;
	while((c=yylex())!='\n') printf("%d\n", c);
} 
