%{
	int nb_mots = 0;
	int nb_mots_ligne = 0;


%}

%s ICI
%%
<INITIAL>([a-z]*) {nb_mots++; BEGIN(ICI);}
<ICI>([a-z]*\ ) {nb_mots++;}
(.)	{return(0);}
%%

int main(){
   int res = yylex();
   printf("Nombre de mots : %d\n", nb_mots);
exit(0);
}

//a améliorer (mettre tous les caractères + faire l'optimisation)
