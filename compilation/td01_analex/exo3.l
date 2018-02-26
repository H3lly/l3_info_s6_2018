%{
	int nba = 0;
	int nbb = 0;

	int compare(char *s1, char *s2){
    		if (s1[0]==s2[0]) return 0;
    		return 1;
	}
%}

%s PAPB PAIB IAIB IAPB
%%

<INITIAL>[a|b] {
	if(compare("a", yytext)==0){
		nba++;
		BEGIN(IAPB);
	}else{
		nbb++;
		BEGIN(PAIB);
	}
}

<PAIB>a|b {
	if(compare("a", yytext)==0){
		nba++;
		BEGIN(IAIB);
	}else{
		nbb++;
		BEGIN(INITIAL);
	}
}
<IAIB>a|b {
	if(compare("a", yytext)==0){
		nba++;
		BEGIN(INITIAL);
	}else{
		nbb++;
		BEGIN(IAPB);
	}
}
<IAPB>a|b  {

	if(compare("a", yytext)==0){
		nba++;
		BEGIN(INITIAL);
	}else{
		nbb++;
		BEGIN(IAIB);
	}
}


(\n)	{return 0;}
%%

int main(){
   int res = yylex();
   printf("a = %d, b = %d\n", nba, nbb);
   if (nba%2==0 && nbb%2==1)
      printf(" mot reconnu \n");
   else
      printf(" mot rejete \n" );
exit(0);
}
