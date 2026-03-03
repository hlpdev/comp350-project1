#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_prime(int n) {
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

int main(int argc, char** argv) {
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);

  printf("\tcount primes instance starting at %d and ending at %d\n", start, end - 1);
  
  int count = 0;
  for (int i = start; i < end; i++) {
    if (is_prime(i)) {
      count++;
    }
  }

  printf("\tcount primes instance for %d to %d is %d\n", start, end - 1, count);
  return count;
}
