#include <string.h>
#include <stdio.h>

char *strfind(const char *s, const char *f) {
  int sLen = strlen(s), fLen = strlen(f);

  char *firstOccur = NULL;
  for (int i = 0; i < (fLen - sLen); i++) {
      firstOccur = &f[i];
      for (int j = 0; j < sLen; j++) {
        if (f[i+j] != s[j]) break;
        else if (j == (sLen-1)) return firstOccur;
      }
  }

  return NULL;
}

int main(void) {
  char a[] = "hello world";

  char *f = strfind("lo", a);

  for (int i = 0; f[i] != '\0'; i++)
    printf("%c", f[i]);

  printf("\n");

  return 0;
}
