int cntlower(char str[], unsigned int len) {
  if (len == 0) return 0;

  int count = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] >= 97 && str[i] <= 122) count++;
  }

  return count;
}
