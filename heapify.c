#include <stdio.h>
#include <stdlib.h>

typedef struct tree *tptr;
typedef struct tree {
  int size;
  int *val;
  tptr p; // parent
  tptr l, r;
} tree;

tptr new_tree(tptr p, int *v) {
  tptr new = (tptr)malloc(sizeof(tree));
  if (v == NULL) {
    new->size = 0;
    new->val = NULL;
  } else {
    new->size = 1;
    new->val = v;
  }
  new->p = p;
  return new;
}

tptr insert(int v, tptr t) {
  if (t->val == NULL) {
    t->val = (int *)malloc(sizeof(int));
    *(t->val) = v;
    t->size = 1;
    t->l = new_tree(t, NULL);
    t->r = new_tree(t, NULL);
    return t;
  }

  t->size += 1;
  if (((t->size-1) & 1) == 0) {
    return insert(v, t->r);
  } else {
    return insert(v, t->l);
  }
}

void bubble_up(tptr n) {
  if (n->p != NULL && ((*n->val) < (*(n->p->val)))) {
    int temp = *n->val;
    *n->val = *(n->p->val);
    *(n->p->val) = temp;
    bubble_up(n->p);
  }
}

void heapify(int ns[], int count, tptr t) {
  if (count == 0) return;

  tptr newNode = insert(ns[0], t);
  bubble_up(newNode);
  ns++;
  heapify(ns, count-1, t);
}

void print(tptr t) {
  if (t == NULL || t->val == NULL) {
    printf("NULL\n");
    return;
  }

  printf("%d\n", *t->val);
  print(t->l);
  print(t->r);
}

int main(void) {
  tptr t = new_tree(NULL, NULL);
  int arr[] = {5,3,2,1,4};
  heapify(arr, 5, t);

  print(t);

  return 0;
}
