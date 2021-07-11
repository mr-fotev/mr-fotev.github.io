#include <cstdio>
#include <cstdlib>

const long long MOD = 1000000000;
const int M5 = 500000000;
char s[10002];

struct Matrix {
  long long a, b, c;
};

Matrix operator *(Matrix p, Matrix q) {
  return {
      (p.a * q.a + p.b * q.b) % MOD,
      (p.a * q.b + p.b * q.c) % MOD,
      (p.b * q.b + p.c * q.c) % MOD,
  };
}

int main() {
  int r = 0, n = 0;

  scanf("%s", s);

  while (s[n])
    r += s[n++];

  int k = strtol(s + (n > 9 ? n - 9 : 0), nullptr, 10) % M5;
  k += ((k - r) % 3 + 3) % 3 * M5;

  Matrix m{1, 0, 1}, a{1, 1, 0};
  while (k) {
    if (k & 1)
      m = m * a;
    k = k >> 1;
    a = a * a;
  }

  printf("%09lld\n", m.b);

  return 0;
}