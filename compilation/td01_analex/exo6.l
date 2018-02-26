%{
%}

%%

<INITIAL>a { printf("b"); }
<INITIAL>b { printf("c"); }
<INITIAL>c { printf("a"); }

.|\n	{printf("\n"); return 0;}
%%

int main(){
	yylex();
	exit(0);
}
