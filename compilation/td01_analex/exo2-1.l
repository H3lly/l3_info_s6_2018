%{

%}


%%
<INITIAL>cabb|cbab|cbb|cbbba     {return(1);}
(.|\n)	{return(0);}
%%

int main(){
   int res = yylex();
   if (res==1)
      printf(" mot reconnu \n");
   else
      printf(" mot rejete \n" );
exit(0);
}
