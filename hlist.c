#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define INF 1000

typedef struct hnode_s hnode_t;
struct hnode_s {
  int val;
  hnode_t *prev, *next;
  hnode_t *bot, *top;
};

typedef struct {
  hnode_t* head;
  int height;
} hlist_t;

hlist_t* hlist_new() {
  hlist_t* l = malloc(sizeof(hlist_t));
  hnode_t* minf = malloc(sizeof(hnode_t));
  hnode_t* pinf = malloc(sizeof(hnode_t));

  minf->val = -INF;
  minf->prev = NULL;
  minf->next = pinf;
  minf->bot = NULL;
  minf->top = NULL;

  pinf->val = +INF;
  pinf->prev = minf;
  pinf->next = NULL;
  pinf->bot = NULL;
  pinf->top = NULL;

  l->height = 1;
  l->head = minf;

  return l;
}

void hlist_free(hlist_t *l) {
  hnode_t *first = l->head->next;
  for (int i = 0; i < l->height; i++) {
    hnode_t *node = first;
    while(node != NULL) {
      node = node->next;
      free(node->prev);
    }
    first = first->bot;
  }
  free(l);
}

int hlist_search(hlist_t *l, int v, hnode_t* path[]) {
  hnode_t *node = l->head;
  for (int i = 0; i < l->height; i++) {
    while (node->val != v && node->next != NULL && node->next->val <= v) node = node->next;
    path[i] = node;
    // printf("%d\n", node->val);
    if (i != l->height - 1) node = node->bot;
  }
  return node->val == v;
}

int hlist_add(hlist_t *l, int v) {
  // hnode_t** path = malloc(sizeof(hlist_t*)*(l->height));
  hnode_t *path[l->height];
  hlist_search(l,v,path);
  int r = 0;
  int i = l->height - 1;
  hnode_t *bot = NULL;
  while (i >= 0 && r < RAND_MAX/2) {
    hnode_t* node = malloc(sizeof(hnode_t));
    node->val = v;
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
  if (i < 0) {
    hnode_t* minf = malloc(sizeof(hnode_t));
    hnode_t* pinf = malloc(sizeof(hnode_t));

    minf->val = -INF;
    minf->prev = NULL;
    minf->next = pinf;
    minf->bot = l->head;
    l->head->top = minf;
    minf->top = NULL;

    pinf->val = +INF;
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

int hlist_remove(hlist_t *l, int v) {
  hnode_t *path[l->height];
  int find = hlist_search(l,v,path);
  if (!find) return 0;
  int i = l->height - 1;
  while (i > 0 && path[i]->val == v) {
    path[i]->prev->next = path[i]->next;
    path[i]->next->prev = path[i]->prev;
    free(path[i]);
    i--;
  }
  if (l->height > 1 && l->head->bot->next->val == INF) {
    hnode_t *nhead = l->head->bot;
    free(l->head->next);
    free(l->head);
    l->head = nhead;
    l->height--;
  }
  return 1;
}

void hlist_print(hlist_t *l) {
  hnode_t *first = l->head;
  for (int i = 0; i < l->height; i++) {
    hnode_t *node = first;
    while(node != NULL) {
      printf("%d - ", node->val);
      node = node->next;
    }
    printf("\n");
    first = first->bot;
  }
}

int main(int args, char *argv[]) {
  srand(time(NULL));
  hlist_t* l = hlist_new();
  for(int i = 1; i < args; i++) {
    hlist_add(l, atoi(argv[i]));
  }
  hlist_print(l);
  hlist_remove(l,3);
  hlist_print(l);
}
