%{
	#include "exo35-yacc.h"
%}

%%
[0-9]+ {yylval = (int)yytext[0]; return NUMBER;}
. {return yytext[0];}
%%

int yywrap(){ return 1;}


