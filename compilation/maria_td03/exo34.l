%{
char s;
%}

%option noyywrap

%%
[0-9]+   {return 0;}
.        {printf ("%s\n", yytext);}
%%

int main(){
int c;
while ((c=yylex())!='\n') printf("%d\n", c);
}

