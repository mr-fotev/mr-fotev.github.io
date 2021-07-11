#include <cstdio>
#include <cstdlib>
#include <iostream> // **
#include <chrono> // **

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
  auto beginAll = std::chrono::steady_clock::now(); // ** start measuring time
  
  scanf("%s", s);
  
  auto beginComp = std::chrono::steady_clock::now(); // ** start measuring computation time only
  
  int r = 0, n = 0;
  
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

  auto endComp = std::chrono::steady_clock::now(); // ** end measuring computation time only
  
  printf("%09lld\n", m.b);

  auto endAll = std::chrono::steady_clock::now(); // ** end measuring time
  
  auto elapsedAll = std::chrono::duration_cast<std::chrono::nanoseconds>(endAll - beginAll); // ** elapsed time
  auto elapsedComp = std::chrono::duration_cast<std::chrono::nanoseconds>(endComp - beginComp); // ** elapsed computation time
  
  std::cout << elapsedAll.count() << "\n" << elapsedComp.count() << "\n"; // **
  
  return 0;
}