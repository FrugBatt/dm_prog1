#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_CAP 100

typedef struct {
  int x, y;
  bool is_free;
  int next, prev;
} elem_t;

typedef struct {
  elem_t tab[MAX_CAP];
  int size;
  int first, last;
} tlist_t;


tlist_t* tlist_new() {
  tlist_t* p = malloc(sizeof(tlist_t));
  p->size = 0;
  p->first = 0;
  p->last = 0;
  for(int i = 0; i < MAX_CAP; i++) {
    p->tab[i].is_free = false;
  }
  return p;
}

int tlist_free(tlist_t* l) {
  int size = sizeof(l);
  free(l);
  return size;
}

int tlist_size(tlist_t* l) {
  return l->size;
}

int tlist_add(tlist_t* l, int x, int y) {
  if (tlist_size(l) == MAX_CAP) return 0;
  int i = 0;
  while (l->tab[i].is_free) i++;
  l->tab[i].x = x;
  l->tab[i].y = y;
  l->tab[i].is_free = true;
  l->tab[i].prev = l->last;
  l->tab[i].next = l->first;
  l->tab[l->last].next = i;
  l->last = i;
  l->size++;
  return 1;
}

int tlist_remove(tlist_t* l, int x, int y) {
  if (tlist_size(l) == 0) return 0;
  int e = l->first;
  for (int i = 0; i<tlist_size(l); i++) {
    if (l->tab[e].x == x && l->tab[e].y == y) {
      l->size--;
      l->tab[e].is_free = false;
      if (tlist_size(l) == 0) {
        l->first = 0;
        l->last = 0;
      } else if (l->first == e) {
        l->first = l->tab[e].next;
      } else if (l->last == e) {
        l->last = l->tab[e].prev;
      } else {
        int prev = l->tab[e].prev;
        int next = l->tab[e].next;
        l->tab[prev].next = next;
        l->tab[next].prev = prev;
      }
      return 1;
    }
    e = l->tab[e].next;
  }
  return 0;
}

int tlist_pop(tlist_t* l) {
  if (tlist_size(l) == 0) return 0;
  int first = l->first;
  l->tab[first].is_free = false;
  l->first = l->tab[first].next;
  l->size--;
  return 1;
}

int tlist_top(tlist_t* l, int *x, int *y) {
  if (tlist_size(l) == 0) return 0;
  *x = l->tab[l->first].x;
  *y = l->tab[l->first].y;
  return 1;
}

int tlist_push(tlist_t* l, int x, int y) {
  if (tlist_size(l) == MAX_CAP) return 0;
  int i = 0;
  while (l->tab[i].is_free) i++;
  l->tab[i].x = x;
  l->tab[i].y = y;
  l->tab[i].is_free = true;
  l->tab[i].next = l->first;
  l->tab[l->first].prev = i;
  l->first = i;
  l->size++;
  return 1;
}

int tlist_swap(tlist_t* l, int i, int j) {
  if (0 > i || 0 > j || i >= MAX_CAP || j >= MAX_CAP) return 0;
  if (!l->tab[i].is_free || !l->tab[j].is_free) return 0;
  // int previ = l->tab[i].prev;
  // int nexti = l->tab[i].next;
  // l->tab[i].prev = l->tab[j].prev;
  // l->tab[i].next = l->tab[j].next;
  // l->tab[j].prev = previ;
  // l->tab[j].next = nexti;
  // if(l->first == i) l->first = j;
  // else if(l->first == j) l->first = i;
  // if(l->last == i) l->last = j;
  // else if (l->last == j) l->last = i;
  if (l->tab[i].next == j) {
    int pi = l->tab[i].prev;
    int nj = l->tab[j].next;
    l->tab[i].prev = j;
    l->tab[i].next = nj;
    l->tab[j].prev = pi;
    l->tab[j].next = i;
  } else if (l->tab[i].next == i) {
    int pj = l->tab[j].prev;
    int ni = l->tab[i].next;
    l->tab[j].prev = i;
    l->tab[j].next = ni;
    l->tab[i].prev = pj;
    l->tab[i].next = j;
  } else {
    int pi = l->tab[i].prev;
    int ni = l->tab[i].next;
    int pj = l->tab[j].prev;
    int nj = l->tab[j].next;
    l->tab[i].prev = pj;
    l->tab[i].next = nj;
    l->tab[j].prev = pi;
    l->tab[j].next = ni;
  }
  if (l->first == i) l->first = j;
  else if (l->first == j) l->first = i;
  if (l->last == i) l->last = j;
  else if (l->last == j) l->last = i;
  // int x = l->tab[i].x;
  // int y = l->tab[i].y;
  // l->tab[i].x = l->tab[j].x;
  // l->tab[i].y = l->tab[j].y;
  // l->tab[j].x = x;
  // l->tab[j].y = y;
  return 1;
}

int tlist_sort(tlist_t* l) {
  for (int i = 0; i < tlist_size(l)-1; i++) {
    int e0 = l->first;
    int e1 = l->tab[e0].next;
    for (int j = 0; j < tlist_size(l)-1-i; j++) {
      int x0 = l->tab[e0].x;
      int y0 = l->tab[e0].y;
      int x1 = l->tab[e1].x;
      int y1 = l->tab[e1].y;
      if (x0*x0 + y0*y0 > x1*x1 + y1*y1) tlist_swap(l,e0,e1);
      e0 = e1;
      e1 = l->tab[e0].next;
    }
  }
  return 1;
}

int tlist_print(tlist_t* l) {
  int e = l->first;
  for (int i = 0; i < tlist_size(l); i++) {
    printf("x: %d, y: %d\n", l->tab[e].x, l->tab[e].y);
    e = l->tab[e].next;
  }
  return tlist_size(l);
}

int main(int args, char *argv[]) {
  tlist_t* l = tlist_new();
  for(int i = 1; i < args; i+=2) {
    tlist_add(l, atoi(argv[i]), atoi(argv[i+1]));
  }
  printf("Liste non triée:\n");
  tlist_print(l);


  tlist_remove(l, 2, 3);
  tlist_remove(l,5,6);
  
  tlist_print(l);

  tlist_sort(l);
  printf("Liste triée:\n");
  tlist_print(l);
  tlist_free(l);
}
