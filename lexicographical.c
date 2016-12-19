#include <stdio.h>

void bubble(char *ns, int size) {
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

#define LENGTH 12

int main(void) {
  char chars[] = "testingagain";
  for (int i = 0; i < LENGTH; i++)
    printf("%c,", chars[i]);

  printf("\n");

  bubble(chars, LENGTH);

  for (int i = 0; i < LENGTH; i++)
    printf("%c,", chars[i]);

  printf("\n");

  return 0;
}
