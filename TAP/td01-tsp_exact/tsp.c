//
//  TSP
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "variables.h"
#include "utils.h"
#include "misc.c"


static void drawTour(point *V, int n, int *P){

  // saute le dessin si le précédent a été fait il y a moins de 20ms
  static unsigned int last_tick = 0;
  if(n<0) { last_tick = 0; n=-n; }  // force le dessin si n<0
  if(last_tick + 20 > SDL_GetTicks()) return;
  last_tick = SDL_GetTicks();

  // gestion de la file d'event
  handleEvent(false);

  // efface la fenêtre
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  // dessine le cycle
  if(P && V){
    selectColor(1,1,1); // couleur blanche
    for (int i = 0 ; i < n ; i++)
      drawLine(V[P[i]], V[P[(i+1) % n]]);
  }
  // dessine les points
  if(V){
    selectColor(1,0,0); // couleur rouge
    for (int i = 0 ; i < n ; i++)
      drawPoint(V[i]);
  }

  // affiche le dessin
  SDL_GL_SwapWindow(window);
}


static void drawPath(point *V, int n, int *path, int len){

  // saute le dessin si le précédent a été fait il y a moins de 20ms
  static unsigned int last_tick = 0;
  if(n<0) { last_tick = 0; n=-n; }  // force le dessin si n<0
  if(last_tick + 20 > SDL_GetTicks()) return;
  last_tick = SDL_GetTicks();

  // gestion de la file d'event
  handleEvent(false);

  // efface la fenêtre
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  // dessine le chemin
  selectColor(0,1,0); // vert
  for (int i = 0 ; i < len-1 ; i++)
    drawLine(V[path[i]], V[path[i+1]]);

  // dessine les points
  selectColor(1,0,0); // rouge
  for (int i = 0 ; i < n ; i++)
    drawPoint(V[i]);

  // affiche le dessin
  SDL_GL_SwapWindow(window);
}


// Génère n points aléatoires du rectangle entier [0,X] × [0,Y] et
// renvoie le tableau des n points ainsi générés et met à jour la
// variable globale vertices[].
static point* generatePoints(int n, int X, int Y) {
  vertices = malloc(n * sizeof(point));
  const double rx = (double) X / RAND_MAX;
  const double ry = (double) Y / RAND_MAX;
  for (int i = 0 ; i < n ; i++){
    vertices[i].x = random() * rx;
    vertices[i].y = random() * ry;
  }
  return vertices;
}

static double dist(point A, point B){
  double dx = A.x-B.x;
  double dy = A.y-B.y;
  return sqrt(dx*dx+dy*dy);
}

static double value(point *V, int n, int *P){
  double v;
  for(int i=0;i<n-1;++i){
    v+=dist(V[P[i]],V[P[i+1]]);
  }
  v+=dist(V[P[n-1]], V[P[0]]);
  return v;
}

static double tsp_brute_force(point *V, int n, int *Q){
  int *tmp=malloc(n*sizeof(int));
  for(int i=0;  i<n; ++i)
    tmp[i]=i; //on lui met 0 1 2 3 4 5 ...
  double max = value(V, n, tmp);
  
  do{
    double res = value(V, n, tmp);
    if(res<max){
      max=res; //si le parcours est plus petit
      for(int i=0; i<n; ++i)
	Q[i] = tmp[i]; //si on a pu faire la permutation, on la garde dans Q.
    }
  }while(NextPermutation(tmp, n));
  free(tmp);
  return max;
}

struct paire {
    int val;
    int use;
};

void decroissant(int *T, double n){
  int max = 0;  
  for(int i=0 ; i<n ; ++i){
    //ici
    if(T[i]>max){
      max=T[i];
    }
  }

  for(int i=0; i<n; ++i){
    T[i]=max;
    max--;
  }
  
}

static void MaxPermutation(int *P, int n, double k){
  int *Q = malloc((n-k)*sizeof(int));
  for(int i=k, j=0; i<n ; i++,  j++)
    Q[j]=P[i];
  decroissant(Q, k);
  for (int i=k,j=0; i<n ; ++i, ++j)
    P[i]=Q[j];
  return;
}


static double value_opt(point *V, int n, int *P,double w){
  double v;
  for(int i=0;i<n-1;++i){
    v+=dist(V[P[i]],V[P[i+1]]);
    if(v>w){
      return -(i+1);
    }
  }
  return v+dist(V[P[n-1]], V[P[0]]);
}

static double tsp_brute_force_opt(point *V, int n, int *Q){
  int *tmp = malloc(n*sizeof(int));
  for(int i=0 ; i<n ; ++i){
    tmp[i] = Q[i];
  }
  double min = value(V, n, tmp);

  do{
    double res = value_opt(V, n, tmp, min);
    //on vérifie 
    if(res>0){ //value_opt peut retourner une valeur négative si c'est déjà la maximum
      if(res<min){
        min=res;
        for(int i=0; i<n ; i++)
          Q[i] = tmp[i];
      }
    } else {
      //printf("%f\n", res);
      MaxPermutation(tmp, n, res);

    }
  } while(NextPermutation(tmp, n));
  return min;
}

// taille initiale de la fenêtre
int width = 640;
int height = 480;

// pour la fenêtre graphique
bool running = true;
bool mouse_down = false;
double scale = 1;












// ----------------- TSP DYN ------------------


/* ==== Programmation dynamique ==== */

// -> à insérer avant main() dans tsp.c
// -> compléter uniquement tsp_prog_dyn()

static int NextSet(int S, int n){
/*
  Cette fonction permet de lister les sous-ensembles de {0,...,n-1}
  par taille croissante, les sous-ensembles étant vu comme des mots
  binaires codés par des entiers (int). Ainsi NextSet(S,n) donne le
  prochain sous-ensemble celui codé par S qui lui est supérieur.

  Plus précisément, la fonction renvoie le plus petit entier
  immédiatement supérieur à S>0 et qui a: (1) soit le même poids que
  S, c'est-à-dire le même nombre de 1 dans son écriture binaire que S;
  (2) soit le poids de S plus 1 s'il n'y en a pas de même poids. La
  fonction renvoie 0 si S est le plus grand entier sur n bits, soit
  S=2^n-1.

  Le principe de l'algorithme est le suivant (NB: S>0): dans S on
  décale complètement à droite le premier bloc de 1 (celui contenant
  le bit de poids faible) sauf le bit le plus à gauche de ce bloc qui
  lui est décalé d'une position à gauche de sa position d'origine. Par
  exemple 1001110 devient 1010011. Si on ne peut pas décaler ce bit à
  gauche c'est que ou bien on est arrivé à 2^n-1 (et on renvoie 0) ou
  bien on doit passer à un poids plus élevé. Dans ce cas on renvoie le
  plus petit entier de poids p, soit 2^p-1, où p est le poids de S
  plus 1.

  Résultats obtenus en itérant S -> NextSet(S,4) à partir de S=1:
   (taille 1) S = 0001 -> 0010 -> 0100 -> 1000 ->
   (taille 2)     0011 -> 0101 -> 0110 -> 1001 -> 1010 -> 1100 ->
   (taille 3)     0111 -> 1011 -> 1101 -> 1110
   (taille 4)     1111 ->
   (taille 0)     0000

  Ainsi, les 16 entiers sur n=4 bits sont parcourus dans l'ordre: 1 ->
  2 -> 4 -> 8 -> 3 -> 5 -> 6 -> 9 -> 10 -> 12 -> 7 -> 11 -> 13 -> 14
  -> 15 -> 0

  L'algorithme prend un temps O(1) car les opérations de shifts (<<)
  et la fonction ffs() sur des int correspondent à des instructions
  machines sur des registres.
*/
  int p1 = ffs(S); // position du bit le plus à droit (commence à 1)
  int p2 = ffs(~(S >> p1)) + p1 - 1; // position du bit le plus gauche du 1er bloc de 1
  if(p2 - p1 + 1 == n) return 0; // cas S=2^n-1
  if(p2 == n) return (1 << (p2 - p1 + 2)) - 1; // cas: poids(S)+1
  return (S & ((-1) << p2)) | (1 << p2) | ((1 << (p2 - p1)) - 1); // cas: poids(S)
}


/* renvoie l'ensemble S\{i} */
static inline int DeleteSet(int S, int i){
  return (S & ~(1 << (i)));
}


/* une cellule de la table */
typedef struct{
  double length; // longueur du chemin minimum D[t][S]
  int pred;      // point précédant t dans la solution D[t][S]
} cell;


int ExtractPath(cell **D, int t, int S, int n, int *Q){
  // Construit dans Q le chemin extrait de la table D depuis la case
  // D[t][S] jusqu'au sommet V[n-1]. Il faut que Q[] soit assez grand
  // Renvoie la taille du chemin

  Q[0]=t;  // écrit le dernier point
  int k=1; // k=taille de Q=nombre de points écrits dans Q
  while(Q[k-1]!=n-1){ // on s'arrête si le dernier point est V[n-1]
    Q[k] = D[Q[k-1]][S].pred;
    S=DeleteSet(S,Q[k-1]);
    k++;
  }
  return k;
}

//static void remplirCaseTableau2D(int *tab, int n, int val, int i, int j){
//  tab[j*n+i+1]=val;
//  }
//int *tmp = malloc(sizeof(int)*(n-1)*pow(2,n-1));

static double tsp_prog_dyn(point *V,int n, int *Q){
  int *checked = malloc(n*sizeof(int));
  for(int i=0; i<n; ++i)
    checked[i] = 0;
  for(int i=0; i<n; ++i){
    double tmp = -1.0;
    int indice = 0;
    for (int j=0; j<n; j++){
      if(i!=j && chacked[j]==0){
        point father = V[Q[i]];
        point current = V[Q[j]];
        double dist = dist(father, current);
        if(res==-1.0||dist<res){
          res=dist;
          indice=i;
        }
      }
    }
    checked[i] = 1;
    if(i!=(n-1))
      Q[i] = indice;
  }

  for(int i=0; i<n; ++i)
    Q[i] = i;
  while(first_flip(V, n, Q)){
    draw_tour(V,n,Q);
    return value(V, n, Q);
  }
}

static int first_flip(point *V, int n, int *P){
  for(int i=0; i<n; ++i){
    for(int j=i+2; j<n; ++j){
      double gain = ( dist(V[P[i]], V[P[j]])+dist(V[P[i+1]], V[P[(j+1)%n]]) ) - ( dist(V[P[i]], V[P[i+1]])+dist(V[P[j]], V[P[(j+1)%n]]));
      if(gain<0){
        int tmp = P[i+1];
        P[i+1] = P[j];
        P[j] = tmp;
        return gain
      }
    }
  }
  return 0;
}

static double old_tsp_prog_dyn(point *V, int n, int *Q){
  
  
  /*
  Version programmation dynamique du TSP. Le résultat (la tournée
  optimale) doit être écrit dans la permutation Q, tableau qui doit
  être alloué avant l'appel. On renvoie la valeur de la tournée
  optimale ou -1 s'il y a eut une erreur (pression de 'q' pour sortir
  de l'affichage). Une fois que votre programme arrivera à calculer
  une tournée optimale (à comparer avec tsp_brute_force() ou
  tsp_brute_force_opt()), il sera intéressant de dessiner à chaque
  fois que possible le chemin courant avec drawPath(V,n,Q). La
  variable "running" indique si l'affichage est actif (ce qui peut
  ralentir le programme), la pression de 'q' faisant passer running à
  faux.

  Attention ! Par rapport au cours, il est plus malin de commencer la
  tournée à partir du point d'indice n-1 (au lieu de 0).

  Pour résumer, D est un tableau de cellules à deux dimensions indexé
  par un sommet t (int) et un ensemble S (un int aussi):

  - D[t][S].length = longueur minimum d'un chemin allant de V[n-1] à
    V[t] qui visite tous les points d'indice dans S

  - D[t][S].pred = l'indice du point précédant V[t] dans le chemin
    ci-dessus de longueur D[t][S].length
*/

  int t,S;

  // déclaration de la table D[t][S] qui comportent (n-1)*2^(n-1) cellules
  cell **D = malloc((n-1)*sizeof(cell*)); // n-1 lignes
  for(t=0; t<n-1; t++) D[t] = malloc((1<<(n-1))*sizeof(cell)); // 2^{n-1} colonnes

  // cas de base, lorsque S={t}, pour t=0..n-2: D[t][S]=d(V[n-1],V[t])
  S=1; // S=00...001 et pour l'incrémenter utiliser NextSet(S,n-1)
  ;;;
  ;;;

  // cas |S|>1 (cardinalité de S > 1): D[t][S] = min_x { D[x][S\{t}] +
  // d(V[x],V[t]) } avec t dans S et x dans S\{t}. NB: pour faire T =
  // S\{t}, utilisez T=DeleteSet(S,t); et pour savoir si x appartient
  // à l'ensemble S testez tout simplement si (S != DeleteSet(S,x)).
  do{

    ;;;
    ;;;

    // ici D[t][S].length et D[t][S].pred viennent d'être calculés.
    // Il reste a extraire le chemin de V[t] à V[n-1] et le mettre
    // dans Q, c'est le rôle de la fonction ExtractPath().

       int k = ExtractPath(D,t,S,n,Q);
       drawPath(V,n,Q,k);
    // }

    S=NextSet(S,n-1);
  }while (S && running);

  double w=-1; // valeur de la tournée optimale, -1 par défaut

  if(running){
    // on a terminé correctement le calcul complet de la table D. Il
    // faut alors calculer la longueur w de la tournée optimale et
    // éventuellement construire la tournée pour la mettre dans Q. NB:
    // si le calcul a été interrompu (pression de 'q' pendant
    // l'affichage) alors ne rien faire et renvoyer -1
    ;;;
    ;;;
  }

  ;;; // pensez à libérer la table D
  return w;
}



// --------------------------------------------

int main(int argc, char *argv[]) {

	initSDLOpenGL();
	srandom(time(NULL));
	TopChrono(0); // initialise tous les chronos

	bool need_redraw = true;
	bool wait_event = true;

	int n = 10;
	point *V = generatePoints(n, width, height);
	int *P = malloc(n * sizeof(int));
	for(int i = 0; i < n; i++) P[i] = i; // première permutation
	drawTour(V, n, NULL); // dessine les points

	/*{
	  TopChrono(1); // départ du chrono 1
	  double w = tsp_brute_force(V,n,P);
	  char *s = TopChrono(1); // s=durée
	  printf("value: %g\n",w);
	  printf("runing time: %s\n",s);
	  drawTour(V, -n, P); // dessine la tournée
	}*/

	{
	  /*
	    TopChrono(1); // départ du chrono 1
	    double w = tsp_brute_force_opt(V,n,P);
	    char *s = TopChrono(1); // s=durée
	    printf("value: %g\n",w);
	    printf("runing time: %s\n",s);
	    drawTour(V, -n, P); // dessine la tournée
	  */
	}

	{
	  
	    TopChrono(1); // départ du chrono 1
	    double w = tsp_prog_dyn(V,n,P);
	    char *s = TopChrono(1); // s=durée
	    printf("value: %g\n",w);
	    printf("runing time: %s\n",s);
	    drawTour(V, -n, P); // dessine la tournée
	  
  }

	// Affiche le résultat et attend (q pour sortir)
	while(running){
	  if (need_redraw) drawTour(V, n, P);
	  need_redraw = handleEvent(wait_event);
	}

	// Libération de la mémoire
	TopChrono(-1);
	free(V);
	free(P);

	cleaning();
	return 0;
}
