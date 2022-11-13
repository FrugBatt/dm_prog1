#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Q1 : type hnode_t
typedef struct hnode_s hnode_t;
struct hnode_s {
  int val; // VALEUR
  int inf; // Noeud +/- infini
  hnode_t *prev, *next; //Pointeurs vers les noeuds adjacents
  hnode_t *bot, *top; //Pointeurs vers les noeuds verticalement adjacents
};

//Q2 : type hlist_t
typedef struct {
  hnode_t* head; //Tête de la liste
  int height; //Hauteur
} hlist_t;

//Q3 : création d'une hlist
hlist_t* hlist_new() {
  hlist_t* l = malloc(sizeof(hlist_t));
  // Création des noeuds infinis
  hnode_t* minf = malloc(sizeof(hnode_t));
  hnode_t* pinf = malloc(sizeof(hnode_t));

  minf->inf = -1;
  minf->prev = NULL;
  minf->next = pinf;
  minf->bot = NULL;
  minf->top = NULL;

  pinf->inf = 1;
  pinf->prev = minf;
  pinf->next = NULL;
  pinf->bot = NULL;
  pinf->top = NULL;

  l->height = 1;
  l->head = minf;

  return l;
}

//Q4: libération d'une liste
void hlist_free(hlist_t *l) {
  hnode_t *n_node = l->head;
  for (int i = 0; i < l->height; i++) {
    hnode_t *node = n_node;
    hnode_t *tmp;
    n_node = n_node->bot;
    while(node != NULL) {
      tmp = node;
      node = node->next;
      free(tmp);
    }
  }
  free(l);
}

//Q5: Recherche (avec chemin)
// Comparaison noeud / valeur (pour la gestion des infinis)
int compare_nv(hnode_t* n, int v) {
  if (n->inf == -1) return -1;
  else if (n->inf == 1) return 1;
  else if (n->val < v) return -1;
  else if (n->val == v) return 0;
  else return 1;
}

int hlist_search(hlist_t *l, int v, hnode_t* path[]) {
  hnode_t *node = l->head;
  for (int i = 0; i < l->height; i++) {
    while (node->val != v && node->next != NULL && compare_nv(node->next, v) <= 0) node = node->next;
    path[i] = node;
    if (i != l->height - 1) node = node->bot;
  }
  return node->inf == 0 && node->val == v;
}

//Q6: ajout dans une liste
int hlist_add(hlist_t *l, int v) {
  hnode_t *path[l->height];
  hlist_search(l,v,path);
  int r = 0;
  int i = l->height - 1;
  hnode_t *bot = NULL;
  // On ajoute un nombre aléatoire de fois la valeur ajoutée
  while (i >= 0 && r < RAND_MAX/2) {
    hnode_t* node = malloc(sizeof(hnode_t));
    node->val = v;
    node->inf = 0;
    node->prev = path[i];
    node->next = path[i]->next;
    node->top = NULL;
    node->bot = bot;

    if(bot != NULL) bot->top = node;
    path[i]->next->prev = node;
    path[i]->next = node;
    bot = node;
    r = rand();
    i--;
  }
  // On ajoute 1 hauteur si jamais on a atteint le haut de la liste
  if (i < 0) {
    hnode_t* minf = malloc(sizeof(hnode_t));
    hnode_t* pinf = malloc(sizeof(hnode_t));

    minf->inf = -1;
    minf->prev = NULL;
    minf->next = pinf;
    minf->bot = l->head;
    l->head->top = minf;
    minf->top = NULL;

    pinf->inf = 1;
    pinf->prev = minf;
    pinf->next = NULL;
    pinf->bot = l->head->next->next;
    l->head->next->next->top = pinf;
    pinf->top = NULL;

    l->height++;
    l->head = minf;
  }
  return 1;
}

//Q7: Suppression dans une liste
int hlist_remove(hlist_t *l, int v) {
  hnode_t *path[l->height];
  int find = hlist_search(l,v,path);
  if (!find) return 0;
  int i = l->height - 1;
  while (i > 0 && path[i]->inf == 0 && path[i]->val == v) {
    path[i]->prev->next = path[i]->next;
    path[i]->next->prev = path[i]->prev;
    free(path[i]);
    i--;
  }
  // On retire la première ligne si il le faut
  if (l->height > 1 && l->head->bot->next->inf == 1) {
    hnode_t *nhead = l->head->bot;
    free(l->head->next);
    free(l->head);
    l->head = nhead;
    l->height--;
  }
  return 1;
}

// Fonction d'affichage d'une hlist_t
void hlist_print(hlist_t *l) {
  hnode_t *first = l->head;
  for (int i = 0; i < l->height; i++) {
    hnode_t *node = first;
    while(node != NULL) {
      if (node->inf == -1) printf("-INF - ");
      else if (node->inf == 1) printf("+INF - ");
      else printf("%d - ", node->val);
      node = node->next;
    }
    printf("\n");
    first = first->bot;
  }
}

//Q8: Fonction main
int main(int args, char *argv[]) {
  // On initialise le générateur de nombres aléatoires
  srand(time(NULL));

  hlist_t* l = hlist_new();
  for(int i = 1; i < args; i++) {
    hlist_add(l, atoi(argv[i]));
  }
  hlist_print(l);
  hlist_free(l);
}
