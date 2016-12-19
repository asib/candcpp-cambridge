#include <stdio.h>

int main(void) {
  {
    typedef struct foo *fptr;
    struct foo {int i;} x = {1};
    fptr p = &x;

    printf("%d\n", ++p->i); // this means do ++((*p).i) - prints 2
    printf("%d\n", p++->i); // this means do (*p).i, then increment p pointer
                            // - prints 2 then next derefence will be undefined
  }

  {
    typedef struct foo *fptr;
    int x = 5;
    struct foo {int *i;} y = {&x};
    fptr p = &y;

    printf("%d\n", *p->i);   // does *((*p).i) - prints 5
    printf("%d\n", *p->i++); // does *(((*p).i)++), so returns current value pointed
                             // to by i, and afterwards, incrememnts the pointer i
                             // so that next access is undefined.
    // Reset
    y.i = &x;

    printf("%d\n", (*p->i)++); // does (*((*p).i))++, so returns value pointed to
                               // by i, then incrememnts the value itself - prints 5
    printf("%d\n", *p->i);     // because above incremented value, prints 6
    printf("%d\n", *p++->i);   // does *(((*p)++).i), so will fetch value pointed
                               // to by i, then afterwards, incrememnt p - prints 6
    printf("%d\n", *p->i);     // undefined, because p was incremented above
  }

  return 0;
}
