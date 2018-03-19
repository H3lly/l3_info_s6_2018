#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "variables.h"
#include "utils.h"
#include "heap.h"

typedef double (*heuristic)(position,position,grid*);

double h0(position s, position t, grid *G){
  return 0.0;
}

double hvo(position s, position t, grid *G){
  return fmax(abs(t.x-s.x),abs(t.y-s.y));
}

typedef struct{
  position pos;        // position (.x,.y) d'un noeud u
  double cost;         // coût[u]
  double score;        // score[u] = coût[u] + h(u,end)
  struct node* parent; // parent[u] = pointeur vers le père, NULL pour start
  int source;
} node;

double weight[]={
    1.0,  // V_FREE
  -99.0,  // V_WALL
    3.0,  // V_SAND
    9.0,  // V_WATER
    2.3,  // V_MUD
    1.5,  // V_GRASS
    0.1,  // V_TUNNEL
};

node* createNode(int x,int y,double cost,double score,node* parent){
    node* n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Not enough memory\n");
    }
    n->cost=cost;
    n->score=score;
    n->pos.x=x;
    n->pos.y=y;
    n->parent=parent;
    return n;
}

node* createNode2(int x,int y,double cost,double score,node* parent,int source){
    node* n=createNode(x,y,cost,score,parent);
    n->source=source;
    return n;
}


int nodeCompare(const void* n1, const void* n2){
    if(((node*)n1)->score - ((node*)n2)->score > 0)
        return 1;
    if(((node*)n1)->score == ((node*)n2)->score)
        return 0;
    return -1;
}



void A_star(grid G, heuristic h){

    node* start=createNode(G.start.x,G.start.y,0,h(G.start,G.end,&G),NULL);
    G.mark[start->pos.x][start->pos.y]=M_FRONT;

    node** P=malloc(G.X*G.Y*sizeof(node*));
    int PLenght=0;
    heap tas=heap_create(4*G.X*G.Y-2*(G.X+G.Y),nodeCompare);
    heap_add(tas,start);

    while(! heap_empty(tas)){
        node* u=heap_pop(tas);
        if(G.mark[u->pos.x][u->pos.y]==M_USED){ //si le sommet actuel est dans P
	  free(u); // on libere u
	  continue; //on saute et reprends la boucle au début
        }


        P[PLenght]=u;PLenght++; //on ajoute le u à P, et on augmente l'indice de P;
        G.mark[u->pos.x][u->pos.y]=M_USED; //on mark u comme appartenant à P;
        drawGrid(G); //on dessine la grille

        if(u->pos.x==G.end.x && u->pos.y==G.end.y){ //Si u = t (si u = G.end)
	  while(u->parent!=NULL){  //tant qu'on a pas fini de faire tout le chemin
	    u=u->parent; //on prend le parent de u
	    G.mark[u->pos.x][u->pos.y]=M_PATH; // on mark u comme étant dans le chemin
            drawGrid(G);
            }
            for(int i=0;i<PLenght;i++){
	      free(P[i]); //on pense à libérer les malloc
            }
            free(P);
            while(! heap_empty(tas)){
	      free(heap_pop(tas)); // on pense à libérer le tas
            }
            heap_destroy(tas);
            return; // FINI !!
        }

        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                /*if((i+j)%2==0){//to only keep 4 neighbours
                    continue;
                }*/
                if(G.mark[u->pos.x+i][u->pos.y+j]==M_USED){
                    continue;
                }
                position pos;
                pos.x=u->pos.x+i;
                pos.y=u->pos.y+j;
                if(G.value[pos.x][pos.y]==V_WALL)
                    continue;

                G.mark[pos.x][pos.y]=M_FRONT;
                double c=u->cost+weight[G.value[pos.x][pos.y]];
                node* newNode=createNode(pos.x,pos.y,c,c+h(pos,G.end,&G),u);
                heap_add(tas,newNode);


            }
        }
        drawGrid(G);

    }
    fprintf(stderr,"No path found\n");
    for(int i=0;i<PLenght;i++){
        free(P[i]);
    }
    free(P);
    heap_destroy(tas);
    return;

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

void A_star2(grid G, heuristic h){

  ;;;
  // Pensez à dessiner la grille avec drawGrid(G) à chaque fois que
  // possible, par exemple, lorsque vous ajoutez un sommet à P mais
  // aussi lorsque vous reconstruisez le chemin à la fin de la
  // fonction. Lorsqu'un sommet passe dans Q vous pourrez le marquer
  // M_FRONT (dans son champs .mark) pour le distinguer des sommets de
  // P (couleur différente).
  ;;;
  // Après avoir extrait un noeud de Q, il ne faut pas le détruire,
  // sous peine de ne plus pouvoir reconstruire le chemin trouvé! Vous
  // pouvez réfléchir à une solution simple pour libérer tous les
  // noeuds devenus inutiles à la fin de la fonction. Une fonction
  // createNode() peut simplifier votre code.
  ;;;
  // Les bords de la grille sont toujours constitués de murs (V_WALL) ce
  // qui évite d'avoir à tester la validité des indices des positions
  // (sentinelle).
  ;;;
    node* start=createNode2(G.start.x,G.start.y,0,h(G.start,G.end,&G),NULL,1);
    node* end=createNode2(G.end.x,G.end.y,0,h(G.end,G.start,&G),NULL,0);
    G.mark[start->pos.x][start->pos.y]=M_FRONT;

    node** P=malloc(G.X*G.Y*sizeof(node*));
    int PLenght=0;
    node** P2=malloc(G.X*G.Y*sizeof(node*));
    int P2Lenght=0;
    heap tas=heap_create(4*G.X*G.Y-2*(G.X+G.Y),nodeCompare);
    heap_add(tas,start);
    heap_add(tas,end);

    while(! heap_empty(tas)){
        node* u=heap_pop(tas);

        if(G.mark[u->pos.x][u->pos.y]==M_USED && u->source==0 || G.mark[u->pos.x][u->pos.y]==M_USED2 && u->source==1){//meeting point
            //TODO end
            node* v=u;
            G.mark[u->pos.x][u->pos.y]=M_PATH;
            drawGrid(G);
            G.mark[v->pos.x][v->pos.y]=M_PATH;
            drawGrid(G);

            while(u->parent!=NULL){
                u=u->parent;
                G.mark[u->pos.x][u->pos.y]=M_PATH;
                drawGrid(G);
            }

            if(v->source){
                for(int k=0;k<P2Lenght;k++){
                    if(abs(P2[k]->pos.x-v->pos.x)<=1 && abs(P2[k]->pos.y-v->pos.y)<=1){
                        if((P2[k]->pos.x+P2[k]->pos.y)%2==0){//to only keep 4 neighbours
                            continue;
                        }
                        v=P2[k];
                        break;
                    }
                }
            }else{
                for(int k=0;k<PLenght;k++){
                    if(abs(P[k]->pos.x-v->pos.x)<=1 && abs(P[k]->pos.y-v->pos.y)<=1){
                        if((P[k]->pos.x+P[k]->pos.y)%2==0){//to only keep 4 neighbours
                            continue;
                        }
                        v=P[k];
                        break;
                    }
                }
            }

            while(v->parent!=NULL){
                v=v->parent;
                G.mark[v->pos.x][v->pos.y]=M_PATH;
                drawGrid(G);
            }

            //free
            for(int i=0;i<PLenght;i++){
                free(P[i]);
            }for(int i=0;i<P2Lenght;i++){
                free(P2[i]);
            }
            free(P2);
            free(P);
            while(! heap_empty(tas)){
                free(heap_pop(tas));
            }
            heap_destroy(tas);
            return;

        }

        if(G.mark[u->pos.x][u->pos.y]==M_USED || G.mark[u->pos.x][u->pos.y]==M_USED2){
            free(u);
            continue;
        }




        if(u->source){
            P[PLenght]=u;
            PLenght++;
            G.mark[u->pos.x][u->pos.y]=M_USED;
        }else{
            P2[P2Lenght]=u;
            P2Lenght++;
            G.mark[u->pos.x][u->pos.y]=M_USED2;
        }
        drawGrid(G);

        /*
        if(u->pos.x==G.end.x && u->pos.y==G.end.y){
            while(u->parent!=NULL){
                u=u->parent;
                G.mark[u->pos.x][u->pos.y]=M_PATH;
                drawGrid(G);
            }
            for(int i=0;i<PLenght;i++){
                free(P[i]);
            }
            free(P);
            while(! heap_empty(tas)){
                free(heap_pop(tas));
            }
            heap_destroy(tas);
            return;
        }
        */

        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                if((i+j)%2==0){//to only keep 4 neighbours
                    continue;
                }
                if(G.mark[u->pos.x+i][u->pos.y+j]==M_USED || G.mark[u->pos.x+i][u->pos.y+j]==M_USED2){
                    continue;
                }
                position pos;
                pos.x=u->pos.x+i;
                pos.y=u->pos.y+j;
                if(G.value[pos.x][pos.y]==V_WALL)
                    continue;

                G.mark[pos.x][pos.y]=M_FRONT;
                double c=u->cost+weight[G.value[pos.x][pos.y]];

                node* newNode;
                if(u->source){
                    newNode=createNode2(pos.x,pos.y,c,c+h(pos,G.end,&G),u,u->source);
                }else{
                    newNode=createNode2(pos.x,pos.y,c,c+h(pos,G.start,&G),u,u->source);
                }
                heap_add(tas,newNode);


            }
        }
        drawGrid(G);

    }
    fprintf(stderr,"No path found\n");
    for(int i=0;i<PLenght;i++){
        free(P[i]);
    }
    for(int i=0;i<P2Lenght;i++){
        free(P2[i]);
    }
    free(P);
    free(P2);
    heap_destroy(tas);
    return;

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
  addRandomBlob(G, V_SAND,   (G.X+G.Y)/15);
  addRandomBlob(G, V_WATER,  (G.X+G.Y)/15);
  addRandomBlob(G, V_MUD,    (G.X+G.Y)/15);
  addRandomBlob(G, V_GRASS,  (G.X+G.Y)/15);
  addRandomBlob(G, V_TUNNEL, (G.X+G.Y)/15);

  scale=round(fmin(width/G.X,height/G.Y)); // zoom courrant = nombre de pixels par unité
  init_SDL_OpenGL(); // à mettre avant le 1er "draw"
  drawGrid(G); // dessine la grille avant l'algo

  A_star2(G,hvo); // heuristique: h0 ou hvo
  update=true;  // pour forcer l'affichage suivant

  while(running){
    drawGrid(G);
    handleEvent(true); // presser 'q' pour quiter
  }

  freeGrid(G);
  cleaning_SDL_OpenGL();

  return 0;
}
