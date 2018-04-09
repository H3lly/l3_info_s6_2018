#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "tp1.h"


// fonction de comparaison (à completer ...) ordonnées de deux points
// A et B: renvoie -1 si "A<B", +1 si "A>B" et 0 sinon
int fcmp_y(const void *A, const void *B){
  const point *pA = A;
  const point *pB = B;
  if(pA->y < pB->y) return -1;
  if(pA->y > pB->y) return 1;
  return 0;
}

// fonction de comparaison (à completer ...) des abscisses de deux
// points A et B: renvoie -1 si "A<B", +1 si "A>B" et 0 sinon
int fcmp_x(const void *A, const void *B){
  const point *pA = A;
  const point *pB = B;
  if(pA->x < pB->x) return -1;
  if(pA->x > pB->x) return 1;
  return 0;
}


// algorithme naïf en O(n^2)
// on suppose que P contient au moins n>=2 points
paire algo_naif(point *P, int n){
  paire res = {P[0], P[1]};
  for(int i=0; i<n-1; ++i){
    for(int j=i+1; j<n; ++j){
      if(dist(P[i],P[j])<dist(res.A, res.B)){
        res.A = P[i];
        res.B = P[j];
      }
    }
  }
  return res;
}



//Algorithme pppprec
//Px = tableau de point trié selon les abcisses
//Py = tableau de point trié selon les coordonnées
//n = nombre de point dans les tableaux
paire rec(point *Px, point *Py, int n){
  //1)
  if(n<=3)
    return algo_naif(Px, n);

  //2)
  int taille = n/2+1;
  point median = Px[taille-1]; //!!!!!!!!!!!!MALLOC

  //3)
  point Ax[taille];
  point Bx[n-taille];
  for(int i=0; i<n; ++i){
    if(i<=taille)
      Ax[i]=Px[i];
    else
      Bx[i-taille]=Px[i];
  }
  point Ay[taille];
  point By[n-taille];
  for(int i=0; i<n; ++i){
    if(i<taille)
      Ay[i]=Py[i];
    else
      By[i-taille]=Py[i];
  }

  //4)
  paire delta_a = rec(Ax, Ay, taille);
  paire delta_b = rec(Bx, By, n-taille);

  //5)
  paire delta;
  if(dist(delta_a.A, delta_a.A)<dist(delta_b.A, delta_b.B)){
    delta = delta_a;
  }else delta = delta_b;

  
  point Sy[n];
  int taille_Sy=0;

  for(int i=0; i<n; ++i){
    if(sqrt(pow(median.x - Py[i].x,2))<dist(delta.A, delta.B)){
      Sy[taille_Sy] = Py[i];
      taille_Sy++;
    }
  }
  /*
  double start = median.x - dist(delta.A, delta.B);//début de S
  double end = median.x + dist(delta.A, delta.B);//fin de S
  
  for(int i=0; i>n; ++i){
    if(Py[i].x>=start && Py[i].x<=end){
      Sy[taille_Sy]=Py[i];
      taille_Sy++;
    }
  }
  */
  if(taille_Sy<2) {
    printf("hey\n");
    return delta;

  }
  
  //6)
  paire minSy = {Sy[0], Sy[1]};
  for(int i=0; i<taille_Sy; ++i){
    for(int j=1; j<8; ++j){
      if(i+j<taille_Sy && dist(Sy[i],Sy[i+j]) < dist(minSy.A,minSy.B)){
        minSy.A = Sy[i];
        minSy.B = Sy[i+j];
      }
    }
  }

  //7)
  if(dist(minSy.A, minSy.B)<dist(delta.A, delta.B)) return minSy;
  return delta;
}


// algorithme récursif en O(nlogn)
// on suppose que P contient au moins n>=2 points
paire algo_rec(point *P, int n){
  point Q[n];//!!!!!!!!!!!!!!!!!!!! MALLOC
  for(int i=0; i<n; ++i) Q[i]=P[i];
  qsort(P, n, sizeof(point), fcmp_x);
  qsort(Q, n, sizeof(point), fcmp_y);

  paire res = rec(P, Q, n);
  return res;
}




int main(int argc, char *argv[]){

  srandom(time(NULL));

  if(argc==2){
    //
    // partie à ne pas modifier
    //
    if(!isdigit(argv[1][0])){ printf("Forgot \"naif\" or \"rec\"?.\n"); exit(1); }
    int n=atoi(argv[1]);
    point *P=point_random(n);
    if((P==NULL)||(n<1)){ printf("Empty point set.\n"); exit(1); }
    printf("%i\n",n);
    for(int i=0;i<n;i++)
      printf(FORM" "FORM"\n",P[i].x,P[i].y);
    return 0;
  }

  if(argc==3){
    //
    // partie à ne pas modifier
    //
    int n;
    paire res={{-1,0},{0,0}};
    point *P=read(argv[1],&n);
    if(!strcmp(argv[2],"naif")) res=algo_naif(P,n);
    if(!strcmp(argv[2],"rec")) res=algo_rec(P,n);
    if(res.A.x<0){ printf("Unknown algorithm.\n"); exit(1); }
    printf("point A: "FORM" "FORM"\n",res.A.x,res.A.y);
    printf("point B: "FORM" "FORM"\n",res.B.x,res.B.y);
    printf("distance: "FORM"\n",dist(res.A,res.B));
    return 0;
  }

  // pour vos tests personnels, si nécessaire, modifier le main()
  // seulement ci-dessous

  ;;;;
  ;;;;
  ;;;;

  return 0;
}
