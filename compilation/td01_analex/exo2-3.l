%{
	int i=0;
	int res = 1;
%}

%%
<INITIAL>cabbb {
	i+=yyleng;
	printf("indice 2 : (%d, 0) | ", i-5);
	res = 0;
	yyless(2);
	i-=3;
	}
<INITIAL>bbc {
	i+=yyleng;
	printf("indice : (%d, 1) | ", i-3);
	res = 0;
	yyless(1);
	i-=2;
	}
<INITIAL>. {i+=1;}


(\n)	{printf("\n"); return res;}
%%

int main(){
   int res = yylex();
   if (res==0)
      printf(" mot reconnu \n");
   else
      printf(" mot rejete \n" );
exit(0);
}


