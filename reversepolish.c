#include <stdio.h>
#include <stdlib.h>

typedef struct stk *stkptr;
typedef struct stk {
  int val;
  stkptr next;
} stack;

stkptr push(stkptr s, int val) {
  stkptr newHead = (stkptr)malloc(sizeof(stack));
  newHead->next = (stkptr)malloc(sizeof(stack));

  newHead->val = val;
  newHead->next = s;
  return newHead;
}

int pop(stkptr *s) {
  int v = (*s)->val;
  stkptr tmp = *s;
  *s = (*s)->next;
  if (tmp != NULL) free(tmp);
  return v;
}

int main(int argc, char *argv[]) {
  stkptr s = NULL;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] >= 48 && argv[i][0] <= 57) {
      s = push(s, argv[i][0] - 48);
    } else {
      int arg1 = pop(&s);
      int arg2 = pop(&s);
      switch (argv[i][0]) {
        case '+':
          s = push(s, arg1+arg2);
          break;
        case '-':
          s = push(s, arg1-arg2);
          break;
        case '*':
          s = push(s, arg1*arg2);
          break;
        case '/':
          s = push(s, arg1/arg2);
          break;
      }
    }
  }

  printf("%d\n", pop(&s));

  return 0;
}
