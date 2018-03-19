#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "variables.h" // contient la structure "grid"
#include "utils.h"
#include "heap.h" // il faut aussi votre code pour heap.c


// Une fonction de type "heuristic" est une fonction h() qui renvoie
// une distance (double) entre une position de départ et une position
// de fin de la grille. La fonction pourrait aussi dépendre de la
// grille, mais on ne l'utilisera pas forcément ce paramètre.
typedef double (*heuristic)(position,position,grid*);


// Heuristique "nulle" pour Dijkstra
double h0(position s, position t, grid *G){
  return 0.0;
}


// Heuristique "vol d'oiseau" pour A*
double hvo(position s, position t, grid *G){
  return fmax(abs(t.x-s.x),abs(t.y-s.y));
}


// Structure "noeud" pour le tas min Q
typedef struct{
  position pos;        // position (.x,.y) d'un noeud u
  double cost;         // coût[u]
  double score;        // score[u] = coût[u] + h(u,end)
  struct node *parent; // parent[u] = pointeur vers le père, NULL pour start
} node;


// Les arêtes, connectant les cases de la grille, sont valuées par
// seulement certaines valeurs possibles. Le poids de l'arête u->v,
// noté w(u,v) dans le cours, entre deux cases u et v voisines est
// déterminé par la valeur de la case finale v. Plus précisément, si
// la case v de la grille contient la valeur C, le poids de u->v
// vaudra weight[C] dont les valeurs exactes sont définies ci-après.
// La liste des valeurs possibles d'une case est donnée dans
// "variables.h": V_FREE, V_WALL, V_WATER, ... Attention !
// weight[V_WALL]<0. Ce n'est pas une valuation correcte car il n'y a
// pas de sommet en position (i,j) si G.value[i][j] = V_WALL.

double weight[]={
    1.0,  // V_FREE
  -99.0,  // V_WALL
    3.0,  // V_SAND
    9.0,  // V_WATER
    2.3,  // V_MUD
    1.5,  // V_GRASS
    0.1,  // V_TUNNEL
};


// Votre fonction A_star(G,h) doit construire un chemin dans la grille
// G entre la position G.start et G.end selon l'heuristique h(). S'il
// n'y a pas de chemin, affichez un simple message d'erreur. Sinon,
// vous devez remplir le champs .mark de la grille pour que le chemin
// trouvé soit affiché plus tard par drawGrid(G). La convention est
// qu'en retour G.mark[i][j] = M_PATH ssi (i,j) appartient au chemin
// trouvé (cf. "variables.h").
//
// Pour gérer l'ensemble P, servez-vous du champs G.mark[i][j] (=
// M_USED ssi (i,j) est dans P). Par défaut, ce champs est initialisé
// partout à M_NULL par initGrid().
//
// Pour gérer l'ensemble Q, vous devez utiliser un tas min de noeuds
// (type node) avec une fonction de comparaison qui dépend du champs
// .score des noeuds. Pensez que la taille du tas Q est au plus la
// somme des degrés des sommets dans la grille. Pour visualiser un
// noeud de coordonnées (i,j) qui passe dans le tas Q vous pourrez
// mettre G.mark[i][j] = M_FRONT.

node* createNode(int x, int y, double cost, double score, node* parent){
  node* n = malloc(sizeof(node));
  if(n==NULL){
    fprintf(stderr, "Not enought memory.\n");
    return 1;
  }
  n->pos.x = x;
  n->pos.y = y;
  n->cost = cost;
  n->score = score;
  n->parent = parent;
  return n;
}

int nodeCompare(const void* n1, const void* n2){
  if( ((node*)n1)->score - ((node*)n2)->score > 0 ) return 1;
  if( ((node*)n1)->score == ((node*)n2)->score > 0 ) return 0;
  return -1;
}

void A_star(grid G, heuristic h){
  // Pensez à dessiner la grille avec drawGrid(G) à chaque fois que
  // possible, par exemple, lorsque vous ajoutez un sommet à P mais
  // aussi lorsque vous reconstruisez le chemin à la fin de la
  // fonction. Lorsqu'un sommet passe dans Q vous pourrez le marquer
  // M_FRONT (dans son champs .mark) pour le distinguer des sommets de
  // P (couleur différente).

  
  node start = createNode(G.start.x, G.start.y, 0, h(G.start, G.end, &G), NULL); //on créé le point de départ
  node **P P=malloc(G.X*G.y*sizeof(node*)); //on créé P
  int P_len = 0; //servira d'indice pour P
  heap Q = heap_create(4*G.x*G.Y-2*(G.X+G.Y), nodeCompare); //on créé Q
  heap_add(Q, start); //on ajoute le point de départ dans Q
  G.mark[start->pos.x][start->pos.y]=M_FRONT; //on marque start comme étant dans Q


  while(!heap_empty(Q)){ //tant que le tas n'est pas vide (tant qu'on a pas parcourus toutes les nodes)
    node *u = heap_pop(Q); //on récupére le sommet dont le chemin déjà parcourus est le min de ts les sommets de Q
                           // comme Q est un tas min, on a pas besoin de calculer le min
                           // on enlève par la même occasion u de Q

    if(G.mark[u->pos.x][u->pos.y]==M_USED){//si le sommet est dans P (aka si le sommet à déja été parcourus)
      free(u);//on le libere u (vu qu'on va pas l'utiliser
      continue;//on reprends la boucle au début sans faire le suite
    }
    int P_len = 0;
    P[P_len] = u;
    P_len++;
    G.mark[u->pos.x][u->pos.y] = M_USED;

    if(u->pos.x==G.end.x && u->pos.y == G.end.y){
      while(u->parent!=NULL){
	u = u->parent;
	G.mark[u->pos.x][u->pos.y] = M_PATH;
	drawGrid(G);
      }
      for(int i=0 ; i<P_len ; ++i) free(P[i]);
      free(P);
      while(!heap_empty(Q)) free(heap_pop(Q));
      heap_destroy(Q);
      return;
    }


    //pour toutes les nodes n de G voisins de u
       //blabal
          //on créé position p;
          //p.x = u->pos.x;
          //p.y = u->pos.y;
          //si n est dans P
             //on CONTINUE
          //si n est un mur, CONTINUE

          //double c= cout[u] + poids de u (affiché dans variables.h, ce sont les sommets qui ont un poids 
          //on les mark tous comme étant dans Q
          //on créé un nouveau node à partir de u
          //on ajoute u dans le tas
    //on dessine la grille
    
  //on affiche l'erreur
    
  }
  
  
  
  // Après avoir extrait un noeud de Q, il ne faut pas le détruire,
  // sous peine de ne plus pouvoir reconstruire le chemin trouvé! Vous
  // pouvez réfléchir à une solution simple pour libérer tous les
  // noeuds devenus inutiles à la fin de la fonction. Une fonction
  // createNode() peut simplifier votre code.
  
  // Les bords de la grille sont toujours constitués de murs (V_WALL) ce
  // qui évite d'avoir à tester la validité des indices des positions
  // (sentinelle).


  
  ;;;
  ;;;
  // Améliorations quand vous aurez fini:
  //
  // (1) Une fois la cible atteinte, afficher son coût ainsi que le
  // nombre de sommets visités (somme des .mark != M_NULL). Cela
  // permettra de comparer facilement les différences d'heuristiques,
  // h0() vs. hvo().
  //
  // (2) Le chemin a tendance à zizaguer, c'est-à-dire à utiliser
  // aussi bien des arêtes horizontales que diagonales (qui ont le
  // même coût), même pour des chemins en ligne droite. Essayer de
  // rectifier ce problème d'esthétique en modifiant le calcul de
  // score[v] de sorte qu'à coût[v] égale les arêtes (u,v)
  // horizontales ou verticales soient favorisées.
  //
  // (3) Modifier votre implémentation du tas dans heap.c de façon à
  // utiliser un tableau de taille variable, en utilisant realloc() et
  // une stratégie "doublante": lorsqu'il n'y a pas plus assez de
  // place dans le tableau, on double sa taille. On peut imaginer que
  // l'ancien paramètre 'n' devienne non pas le nombre maximal
  // d'éléments, mais la taille initial du tableau (comme par exemple
  // n=4).
  //
  // (4) Gérer plus efficacement la mémoire en libérant les noeuds
  // devenus inutiles.
  //
  ;;;
}


// constantes à initialiser avant init_SDL_OpenGL()
int width=640, height=480; // dimensions initiale de la fenêtre
int delay=80; // presser 'a' ou 'z' pour accélèrer ou ralentir l'affichage, unité = 1/100 s

int main(int argc, char *argv[]){

  unsigned t=time(NULL)%1000;
  srandom(t);
  printf("seed: %u\n",t); // pour rejouer la même grille au cas où

  // tester les différentes grilles ...
  
  //grid G = initGridFile("mygrid.txt"); // grille à partir d'un fichier
  //grid G = initGridPoints(320,240,V_FREE,1); // grille uniforme
  //grid G = initGridPoints(32,24,V_WALL,0.2); // grille de pts aléatoires de type donné
  grid G = initGridLaby(30,20,5); // labyrinthe aléatoire

  // Pour ajouter à G des "régions" de différent types:

  //addRandomBlob(G, V_WALL,   (G.X+G.Y)/20);
  //addRandomBlob(G, V_SAND,   (G.X+G.Y)/15);
  //addRandomBlob(G, V_WATER,  (G.X+G.Y)/15);
  //addRandomBlob(G, V_MUD,    (G.X+G.Y)/15);
  //addRandomBlob(G, V_GRASS,  (G.X+G.Y)/15);
  //addRandomBlob(G, V_TUNNEL, (G.X+G.Y)/15);
  
  scale=round(fmin(width/G.X,height/G.Y)); // zoom courrant = nombre de pixels par unité
  init_SDL_OpenGL(); // à mettre avant le 1er "draw"
  drawGrid(G); // dessine la grille avant l'algo

  A_star(G,hvo); // heuristique: h0 ou hvo
  update=true;  // pour forcer l'affichage suivant

  while(running){
    drawGrid(G);
    handleEvent(true); // presser 'q' pour quiter
  }

  freeGrid(G);
  cleaning_SDL_OpenGL();

  return 0;
}
