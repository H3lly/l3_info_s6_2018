%{
%}

%%

<INITIAL>(aa)(aa)*b { printf("[%s]", yytext); }
<INITIAL>[a|b] { printf("[%s]", yytext); }
<INITIAL>(ab)(ab)*b { printf("[%s]", yytext); }

.|\n	{printf("\n"); return 0;}
%%

int main(){
	yylex();
	exit(0);
}
