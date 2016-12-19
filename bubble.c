#include <stdio.h>

void bubble(int *ns, int size) {
  if (size <= 1) return;

  int numSwapped = 1;
  while (numSwapped > 0) {
    numSwapped = 0;

    for (int i = 0, j = 1; j < size; i++, j++) {
      if (ns[i] > ns[j]) { // swap
        int tmp = ns[i];
        ns[i] = ns[j];
        ns[j] = tmp;
        numSwapped++;
      }
    }
  }
}

int main(void) {
  int integers[] = {5,4,3,2,1};
  for (int i = 0; i < 5; i++)
    printf("%d,", integers[i]);

  printf("\n");

  bubble(integers, 5);

  for (int i = 0; i < 5; i++)
    printf("%d,", integers[i]);

  printf("\n");

  return 0;
}
