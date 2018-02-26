%{

	int compare(char s1, char s2){
    		if (s1==s2) return 0;
    		return 1;
	}
%}

%%

<INITIAL>(aa)(aa)*b {
	char *str = malloc(yyleng*(sizeof(char)));
	char *tmp = malloc(yyleng*(sizeof(char)));
	strcpy(tmp, yytext);
	for(int i=0;i<yyleng;++i){
		if(compare(tmp[i],'a')==0) str[i] = 'b';
		if(compare(tmp[i],'b')==0) str[i] = 'c';
	}
	printf("[%s]", str);
}
<INITIAL>[a|b] {
	printf("[%s]", yytext);
}
<INITIAL>(ab)(ab)*b {
	char *str = malloc(yyleng*(sizeof(char)));
	char *tmp = malloc(yyleng*(sizeof(char)));
	strcpy(tmp, yytext);
	for(int i=0;i<yyleng;++i){
		if(compare(tmp[i],'a')==0) str[i] = 'c';
		if(compare(tmp[i],'b')==0) str[i] = 'd';
	}
	printf("[%s]", str);
}

.|\n	{printf("\n"); return 0;}
%%

int main(){
	yylex();
	exit(0);
}
