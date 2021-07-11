#include <iostream>
#include <iomanip>
#include <string>
#include <chrono> // **
using namespace std;

uint_fast32_t mod_1_5G(const string &strn)
{
  int len = strn.size(), sch = 0;
  for (int i = 0; i <= len - 10; ++i)
    sch += strn[i] - '0';
  uint_fast32_t res = (sch % 3 * 10 + strn[len - 9] - '0') % 15;
  for (int i = len - 8; i < len; ++i)
    (res *= 10) += strn[i] - '0';
  return res;
}

uint_fast64_t sqr(uint_fast64_t n)
{
  return n * n;
}

void f_mod_1G_rec(uint_fast32_t n, uint_fast32_t *triad)
{
  uint_fast32_t f_to51[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89,
    144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657,
    46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309,
    3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986,
    102334155, 165580141, 267914296, 433494437, 701408733, 134903170,
    836311903, 971215073, 807526976, 778742049, 586269025, 365011074 };
  if (n < 25)
    triad[1] = f_to51[n];
  else if (n < 51)
    for (int i = 0; i < 3; ++i)
      triad[i] = f_to51[n - 1 + i];
  else
  {
    const uint_fast32_t c1G = 1000000000;
    uint_fast32_t half[3];
    f_mod_1G_rec(n / 2, half);
    uint_fast32_t feven = half[1] * (uint_fast64_t(2) * half[0] + half[1]) % c1G;
    uint_fast32_t fodd = (sqr(half[1]) + sqr(half[2])) % c1G;
    if (n % 2)
    {
      triad[0] = feven;
      triad[1] = fodd;
      triad[2] = (feven + fodd) % c1G;
    }
    else
    {
      triad[1] = feven;
      triad[2] = fodd;
      triad[0] = (c1G + fodd - feven) % c1G;
    }
  }
}

uint_fast32_t f_mod_1G(uint_fast32_t n)
{
  uint_fast32_t tr[3];
  f_mod_1G_rec(n, tr);
  return tr[1];
}

int main()
{
  auto beginAll = std::chrono::steady_clock::now(); // ** start measuring time
  
  string num;
  cin >> num;
  
  auto beginComp = std::chrono::steady_clock::now(); // ** start measuring computation time only
  
  uint_fast32_t fib_ = f_mod_1G(mod_1_5G(string("00000000") + num));
  
  auto endComp = std::chrono::steady_clock::now(); // ** end measuring computation time only
  
  cout << setw(9) << setfill('0')
       << fib_ << endl;
  
  auto endAll = std::chrono::steady_clock::now(); // ** end measuring time
  
  auto elapsedAll = std::chrono::duration_cast<std::chrono::nanoseconds>(endAll - beginAll); // ** elapsed time
  auto elapsedComp = std::chrono::duration_cast<std::chrono::nanoseconds>(endComp - beginComp); // ** elapsed computation time
  
  std::cout << elapsedAll.count() << "\n" << elapsedComp.count() << "\n"; // **
  
  return 0;
}
