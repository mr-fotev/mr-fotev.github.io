#include <iostream>
#include <iomanip>
#include <chrono> // **
#define MOD 1000000000
#define MOD2 3000000000000000000

using namespace std;

bool aBits[50];
int iBits = 0;

int main(){
	auto beginAll = std::chrono::steady_clock::now(); // ** start measuring time
	
	string sIn;
	cin >> sIn;
	
	auto beginComp = std::chrono::steady_clock::now(); // ** start measuring computation time only
	
	int iSize = sIn.size(), i;
	
	int m3 = 0, m5 = 0;
	
	for (i = 0; i < iSize - 8; i++){
		m3 += sIn[i];
	}
	
	i = iSize - 8;
	if (i < 0) i = 0;
	
	m3 = (m3 - i*'0') % 3;
	
	if (iSize > 8){
		m5 = (sIn[iSize - 9] - '0') % 5;
	}

	int m = m5 + 5*((3 + m5 - m3) % 3);
	
	while (i < iSize){
		m = 10*m + sIn[i++] - '0';
	}
	
	while (m > 0){
		aBits[iBits++] = m % 2;
		m /= 2;
	}
	
	long long a = 0, b = 1, x = 2, aa, bb, t;
	
	while (iBits > 0){
		aa = a*a;
		bb = b*b;
		
		a = (MOD2 + 2*bb - 3*aa - x) % MOD;
		b = (aa + bb) % MOD;
		
		if (aBits[--iBits]){
			t = (a + b) % MOD;
			a = b;
			b = t;
			x = -2;
		}
		else{
			x = 2;
		}
	}

	auto endComp = std::chrono::steady_clock::now(); // ** end measuring computation time only
    
	cout << setw(9) << setfill('0') << a << "\n";
	
	auto endAll = std::chrono::steady_clock::now(); // ** end measuring time

	auto elapsedAll = std::chrono::duration_cast<std::chrono::nanoseconds>(endAll - beginAll); // ** elapsed time
	auto elapsedComp = std::chrono::duration_cast<std::chrono::nanoseconds>(endComp - beginComp); // ** elapsed computation time

	std::cout << elapsedAll.count() << "\n" << elapsedComp.count() << "\n"; // **
	
	return 0;
}