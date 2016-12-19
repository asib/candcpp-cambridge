#include <stdio.h>

template<int N, int D> struct test_divisors {
  static const bool result = (N%D) && test_divisors<N,D-1>::result;
};

template<int N> struct test_divisors<N,1> {
  static const bool result = true;
};

template<int N> struct is_prime {
  static const bool result = test_divisors<N,N/2>::result;
};

int main(void) {
  struct is_prime<5> x;
  printf("%d\n", x.result);
  return 0;
}
