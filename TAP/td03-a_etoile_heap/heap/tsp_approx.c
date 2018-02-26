//
//  TSP - APPROXIMATION / HEURISTIQUES
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


static point* generatePoints(int n, int X, int Y) {
  // Génère n points aléatoires du rectangle entier [0,X] × [0,Y] et
  // renvoie le tableau des n points ainsi générés et met à jour la
  // variable globale vertices[].

  vertices = malloc(n * sizeof(point));
  const double rx = (double) X / RAND_MAX;
  const double ry = (double) Y / RAND_MAX;
  for (int i = 0 ; i < n ; i++){
    vertices[i].x = random() * rx;
    vertices[i].y = random() * ry;
  }
  return vertices;
}

static void generateCircle(point V[], int n, int p, point c, double r){
  // Ajoute p points au tableau V qui en comporte déjà n. Les p
  // nouveaux points sont placés aléatoirement sur le cercle de centre
  // c et de rayon r et insérés aléatoirement parmi les p points déjà
  // présents de V. Le tableau V doit être de longueur au moins n+p.

  const double K=2.0*M_PI/RAND_MAX;
  double a;
  int i;
  for(i=0; i<p; i++){ // p point sur le cercle
    a=K*random();
    V[n+i].x = c.x + r*cos(a);
    V[n+i].y = c.y + r*sin(a);
  }
  Permute(V,n+p); // permute aléatoirement tous les points de V
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

/* =============== TSP FLIP ================ */
void reverse(int *T, int p, int q){
  int *X =malloc((q-p+1)*sizeof(int));
  int taille = q-p+1;
  int qq=q;
  for(int i=0; i<taille; ++i){
    X[i]=T[qq];
    qq--;
  }

  int pp=p;
  for(int i=0; i<taille; ++i){
    T[pp]=X[i];
    pp++;
  }
    free(X);
}

double first_flip(point *V, int n, int *P){
  for(int i=0; i<n; ++i){
    for(int j=i+2; j<n; j++){
      point a=V[P[i]];
      point b=V[P[j]];
      point aa=V[P[(i+1)%n]];
      point bb=V[P[(j+1)%n]];
      double gain =( dist(a,b) + dist(aa,bb))-(dist(a,aa)+dist(b,bb));
      if(gain<0){
	reverse(P,i+1,j);
	return -gain;
      }
    }
  }
  return 0.0;		    
}

static double tsp_flip(point *V, int n, int *P){
  for(int i=0; i<n; ++i)
    P[i]=i;
  while(first_flip(V,n,P)>0)
    drawTour(V,n,P);
  return value(V,n,P);
}

/* =============== TSP GREEDY ================ */
static double tsp_greedy(point *V, int n, int *P){
  // ...
  return 0.0;
}

// taille initiale de la fenêtre
int width  = 640;
int height = 480;

// pour la fenêtre graphique
bool running    = true;
bool mouse_down = false;
double scale    = 1;


int main(int argc, char *argv[]) {

  initSDLOpenGL();
  srandom(time(NULL));
  TopChrono(0); // initialise tous les chronos

  int n = (argv[1] && atoi(argv[1])) ? atoi(argv[1]) : 400;
  point *V = generatePoints(n, width, height);
  int *P = malloc(n * sizeof(int));
  for(int i=0; i<n; i++) P[i]=i; // tournée de départ

  {
    drawTour(V, n, NULL); // dessine les points
    TopChrono(1); // départ du chrono 1
    double w = tsp_flip(V,n,P);
    char *s = TopChrono(1); // s=durée
    printf("value: %g\n",w);
    printf("runing time: %s\n",s);
    if(w>0) drawTour(V, -n, P); // force le dessin de la tournée
    else drawTour(V, -n, NULL); // si pas de tournée 
  }

  sleep(1); // attend 1 seconde

  double r=(width+height)/4.0;
  int p=n/2;
  point c;
  c.x=width/2.0, c.y=height/2.0;
  generateCircle(V,0,p,c,r); // ajoute un grand cercle
  generateCircle(V,p,n-p,c,r/2.0); // ajoute un petit cercle

  {
    drawTour(V, n, NULL); // dessine les points
    TopChrono(1); // départ du chrono 1
    double w = tsp_flip(V, n, P);
    char *s = TopChrono(1); // s=durée
    printf("value: %g\n",w);
    printf("runing time: %s\n",s);
    if(w>0) drawTour(V, -n, P); // force le dessin de la tournée
    else drawTour(V, -n, NULL); // si pas de tournée 
  }

  // Affiche le résultat et attend (q pour sortir)
  bool has_changed = true;
  bool wait_event  = true;
  
  while(running){
    wait_event = true;

    if (first_flip(V,n,P) > 0)
      wait_event = false;

    drawTour(V, n, P);
    has_changed = handleEvent(wait_event);
  }

  // Libération de la mémoire
  TopChrono(-1);
  free(V);
  free(P);

  cleaning();
  return 0;
}
