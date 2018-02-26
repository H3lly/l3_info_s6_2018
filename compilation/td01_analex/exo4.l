%{
	int nba = 0;
	int nbb = 0;
	int nbc = 0;
	int res = 0;

	int compare(char *s1, char *s2){
    		if (s1[0]==s2[0]) return 0;
    		return 1;
	}
%}

%%

<INITIAL>[a|b|c] {
	if(compare("a", yytext)==0) nba++;
	else if(compare("b", yytext)==0) nbb++;
	else nbc++;
}
.|\n	{return 0;}
%%

int main(){
   int res = yylex();
   if (nba == nbb && nbb == nbc)
      printf(" mot reconnu \n");
   else
      printf(" mot rejete \n" );
exit(0);
}
