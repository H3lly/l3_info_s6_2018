/* ==== Approximation MST ====*/

// -> à insérer avant main() dans tsp_approx.c
// -> compléter uniquement tsp_mst() en fin de fichier

// vous pourrez changer la boucle principale à la fin du main() par
// quelque chose comme ceci (vous risquez d'avoir une erreur à propos
// de "has_changed" qu'il faut changer en "new_redraw"):
//
// graph M=createGraph(n);
// ...
// ...
// bool new_redraw=true;
// while(running){
//  wait_event=true;
//  if(new_redraw) tsp_mst(V,n,P,M);
//  if(first_flip(V,n,P)>0) wait_event=false;
//  drawGraph(V,n,P,M);
//  new_redraw=handleEvent(wait_event);
// }
// freeGraph(M);


typedef struct{ // un graphe
  int n;        // n=nombre de sommets
  int *deg;     // deg[u]=nombre de voisins du sommets u
  int **list;   // list[u][i]=i-ème voisin de u
} graph;


// Crée un graphe à n sommets avec des listes de tailles n.
// Attention !  G.deg[u] n'est pas initialisé
graph createGraph(int n){
  graph G;
  G.n = n;
  G.deg = malloc(n*sizeof(int));
  G.list = malloc(n*sizeof(int*));
  for (int u=0; u<n; u++)
    G.list[u] = malloc(n*sizeof(int)); // taille n par défaut
  return G;
}


// Libère les listes de G
void freeGraph(graph G){
  for(int u=0; u<G.n; u++) free(G.list[u]);
  free(G.list);
  free(G.deg);
}


// Ajoute l'arête u-v au graphe G de manière symétrique
void addEdge(graph G, int u, int v){
  G.list[u][G.deg[u]++] = v;
  G.list[v][G.deg[v]++] = u;
}


// Dessine le graphe G, les points V et la tournée définie par Q
static void drawGraph(point *V, int n, int *Q, graph G) {

  // Saute le dessin si le précédent a été fait il y a moins de 20ms
  static unsigned int last_tick = 0;
  if (last_tick + 20 > SDL_GetTicks()) return;
  last_tick = SDL_GetTicks();

  // Gestion de la file d'event
  handleEvent(false);

  // Efface la fenêtre
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  // Dessin
  glLineWidth(5.0);
  selectColor(0,0.4,0);
  for(int i=0; i<n; i++)
    for(int j=0; j<G.deg[i]; j++)
      if(i < G.list[i][j])
	drawLine(V[i], V[G.list[i][j]]);
  glLineWidth(1.0);
  selectColor(1,1,1);
  for(int i=0; i<n; i++)
    drawLine(V[Q[i]], V[Q[(i+1) % n]]);

  // Rouge
  selectColor(1,0,0);
  for(int i = 0; i<n; i++) drawPoint(V[i]);

  // Affiche le dessin
  SDL_GL_SwapWindow(window);
}


typedef struct{  // une arête u-v avec un poids
  int u,v;       // extrémités de l'arête u-v
  double weight; // poids de l'arête u-v
} edge;


// Comparaison du poids de deux arêtes pour qsort()
// qsort() est une fonction de la libraire standard du C
// ne pas hésiter à utiliser le "man"
int compEdge(const void *p1, const void *p2){
  const edge *e1 = p1;
  const edge *e2 = p2;
  return (int)(e1->weight - e2->weight);
}


// Fusionne les arbres de racines x et y
// (avec l'heuristique basée sur la hauteur)
void Union(int x, int y, int *parent, int *height){
  if(height[x]>height[y]) parent[y]=x;
  else{
    parent[x]=y;
    if(height[x]==height[y]) height[y]++;
  }
}


// Renvoie la racine de l'arbre de x
// (avec l'heuristique de la compression de chemin)
int Find(int x, int *parent){
  if(x!=parent[x]) parent[x]=Find(parent[x],parent);
  return parent[x];
}


// Calcule dans le tableau Q l'ordre de première visite des sommets du
// graphe G selon un parcours de profondeur d'abord à partir du sommet
// u. Le paramètre p est le sommet parent de u. On doit avoir p<0 si u
// est l'origine du parcours (premier appel).

void dfs(graph G, int u, int *Q, int p) {
  static int t; // t = temps ou indice (variable globale) du tableau Q
  if(p<0) t=0;
  Q[t++] = u;
  for(int i=0; i<G.deg[u]; i++)
    if(G.list[u][i] != p) dfs(G, G.list[u][i], Q, u);
}


// Cette fonction (à compléter) doit calculer deux choses (à partir de
// V et n):
//
// 1. Le graphe T, soit l'arbre couvrant V de poids minimum
// 2. La tournée Q, soit l'ordre de visite selon le DFS de T
//
// NB: Q et T doivent être créés et libérés par l'appelant.

double tsp_mst(point *V, int n, int *Q, graph T) {
  // Algorithme:
  // 1. remplir puis trier le tableau d'arêtes
  // 2. répéter pour chaque arête u-v, mais pas plus de n-1 fois:
  //    si u-v ne forme pas un cycle dans T (<=> u,v dans des composantes différentes)
  //    alors ajouter u-v au graphe T
  // 3. calculer dans Q le DFS de T

  // E = tableau de toutes les arêtes définies à partir des n points de V
  edge *E = malloc(...);

  ;;;

  // initialisation pour Union-and-Find
  int *parent = malloc(n*sizeof(int)); // parent[x]=parent de x (=x si racine)
  int *height = malloc(n*sizeof(int)); // height[x]=hauteur de l'arbre de racine x
  for(int x=0; x<n; x++){
    parent[x]=x; // chacun est racine de son propre arbre
    height[x]=0; // hauteur nulle
  }

  ;;;

  // libère les tableaux
  free(parent);
  free(height);
  free(E);

  dfs(T,0,Q,-1); // calcule Q grâce au DFS à partir du sommet 0 de T
  return value(V,n,Q); // renvoie la valeur de la tournée
}
