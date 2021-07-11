// compile with: -O3 -std=c++17
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono> // **

using namespace std;

// fnction to calculate n-th fib number mod something
//
// https://en.wikipedia.org/wiki/Fibonacci_number#Matrix_form
// gives us
// (Fn+1)     (1  1)^n   (1)
// |    |  =  |    |   * | |
// (Fn+2)     (1  0)     (0)
//
// then use optimal integer power calculation
// (note that the mods and uint64 conversions only work for mod upto 2^32/3, but this covers 1e9)
uint32_t fib(const uint32_t n, const uint32_t mod) {
    static constexpr uint32_t max = 10'000'000;
    // cache calculated fibs up to 10 mil
    // duplications above that should be minimal, anyway
    static uint32_t cache[max];

    if (n <= 1) return n;
    if (n < max && cache[n]) return cache[n];

    uint32_t ret;
    if (n % 2) {
        auto hn = (n+1)/2;
        static auto sq = [](uint64_t t) -> uint64_t { return t*t; };
        ret = (sq(fib(hn, mod)) + sq(fib(hn-1, mod))) % mod;
    }
    else {
        auto hn = n/2;
        auto hnfib = fib(hn, mod);
        ret = (uint64_t(2*fib(hn-1, mod) + hnfib) * uint64_t(hnfib)) % mod;
    }

    if (n < max) cache[n] = ret;
    return ret;
}

int main() {
    auto beginAll = std::chrono::steady_clock::now(); // ** start measuring time
	
	// calculating Pisano period for 1e9 gives us 1.5e9
    // (using formula from https://oeis.org/A001175)
    // so calculate the input mod 1.5e9 while reading
    const uint32_t period = []() -> uint32_t {
        const uint32_t divisor = 1'500'000'000; // pisano period
        uint64_t mod = 0; // input mod divisor
        while(true) {
            if(uint32_t digit = cin.get() - '0'; digit <= 9) {
                mod = (mod * 10 + digit) % divisor;
            }
            else break;
        }
        return mod;
    }();
	
	auto beginComp = std::chrono::steady_clock::now(); // ** start measuring computation time only
	
	uint32_t fib_ = fib(period, 1'000'000'000);
	
	auto endComp = std::chrono::steady_clock::now(); // ** end measuring computation time only

    cout << setw(9) << setfill('0') << fib_ << '\n';
	
	auto endAll = std::chrono::steady_clock::now(); // ** end measuring time

	auto elapsedAll = std::chrono::duration_cast<std::chrono::nanoseconds>(endAll - beginAll); // ** elapsed time
	auto elapsedComp = std::chrono::duration_cast<std::chrono::nanoseconds>(endComp - beginComp); // ** elapsed computation time

	std::cout << elapsedAll.count() << "\n" << elapsedComp.count() << "\n"; // **

    return 0;
}
