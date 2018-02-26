%{
	int i=0;
	int res = 1;
%}

mots [abc]*
%%
<INITIAL>cabb	{
	i+=yyleng;
	printf("indice : (%d, 0) | ", i-4);
	res = 0;
	}
<INITIAL>cbab	{
	i+=yyleng;
	printf("indice : (%d, 1) | ", i-4);
	res = 0;
	}
<INITIAL>cbb[a|c] {
	i+=yyleng;
	printf("indice : (%d, 2) | ", i-4);
	res = 0;
	}
<INITIAL>cbbba  {
	i+=yyleng;
	printf("indice : (%d, 3) | ", i-5);
	res = 0;
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
