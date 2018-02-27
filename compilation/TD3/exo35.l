%{
%}

%%
[0-9]+      {yyval = atoi(yytext); return NUMBER;}
.return *yytext;
%%

int yywrap(){
return 1;
}
