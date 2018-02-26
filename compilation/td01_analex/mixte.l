/* mixte.l */
%{
 int nb;
 int nd;
%}
%s Q0 Q1

%%
<INITIAL>ab*    {BEGIN Q1;nb=nb+yyleng-1;}
<Q0>ab*         {BEGIN Q1;nb=nb+yyleng-1;}
<Q1>cd*         {BEGIN Q0;nd=nd+yyleng-1;}
<INITIAL,Q0><<EOF>> {return(1);}
<Q1><<EOF>>         {return(0);}
.        {;}
\n       {;}

%%

int main()
{int res;
res=yylex();
if (res==1)
    printf(" mot reconnu \n");
else
printf(" mot rejete \n" );
printf("nombre de b %d, nombre de d %d",nb,nd);
exit(0);
}



