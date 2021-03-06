/*
	Четири версии на медиана за масив с random non-negative integers и малък тест с 2 000 000 ~ 2 500 000 елемента:
		1) Quick_Select + Median_Of_Medians - O(n)
		2) Quick_Select с елемента в средата - очакване: O(n)
		3) Сортиране и извеждане на средата - O(n log n)
		4) Quick_Select с двоен pivot - очакване: O(n)

	Примерни резултати в мс за 100 теста (сред - мин - макс)
		1) 222.09 - 187 - 266
		2) 25.21 - 15 - 47
		3) 268.23 - 234 - 312
		4) 28.05 - 15 - 51
*/

#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#define N 3000000
using namespace std;

/*
	Quick_Select + Median_Of_Medians за масива A
*/

int A[N];

// медиана на парче с дължина до 5 - insertion sort
int med5(int l, int r)
{
	for (int i = l + 1; i <= r; i++)
	{
		int v = A[i], j = i;
		for (; j > 0 && A[j - 1] > v; j--) A[j] = A[j - 1];
		A[j] = v;
	}

	return (l + r) / 2;
}

int med_of_meds(int l, int r, int K)
{
	while (true)
	{
		if (l == r) return l;

		// избиране на pivot
		int piv;

		if (r - l < 5) piv = med5(l, r);
		else
		{
			int s = l;
			for (int i = l; i <= r; i += 5)
			{
				int rpart = i + 4;
				if (rpart > r) rpart = r;

				int m = med5(i, rpart);
				swap(A[m], A[s++]);
			}

			piv = med_of_meds(l, s, (l + s) / 2);
		}

		// partition с pivot-а

		swap(A[piv], A[r]);

		int ind = l;
		for (int i = l; i < r; i++) if (A[i] < A[r]) swap(A[i], A[ind++]);

		swap(A[ind], A[r]);

		// край на partition-а

		if (K == ind) return K;
		
		if (K < ind) r = ind - 1; else l = ind + 1;
	}
}

/*
	Quick_Select с partition в средата за масив B
*/

int B[N];

int quick_select(int l, int r, int K) // l <= K <= r
{
	while (true)
	{
		if (l == r) return l;
		
		// partition със средата

		swap(B[(l + r) / 2], B[r]);

		int ind = l;
		for (int i = l; i < r; i++) if (B[i] < B[r]) swap(B[i], B[ind++]);

		swap(B[ind], B[r]);

		// край на partition-а

		if (K == ind) return K;

		if (K < ind) r = ind - 1; else l = ind + 1;
	}
}

/*
	Този масив е за сортирането
*/

int C[N];

/*
	Quick_Select с два pivot-а за масив D
*/	

int D[N];

int quick_select_3(int l, int r, int K) // l <= K <= r
{
	while (true)
	{
		if (l == r) return l;
		
		// partition с двата pivot-а - l и r

		if (D[l] > D[r]) swap(D[l], D[r]);

		int low = l + 1, mid = low, high = r - 1;
		
		while (mid <= high)
		{
			if (D[mid] < D[l])
			{
				swap(D[low], D[mid]);
				low++;
				mid++;
			}
			else if (D[mid] >= D[r])
			{
				swap(D[high], D[mid]);
				high--;
			}
			else mid++;
		}
		
		low--;
		swap(D[l], D[low]);

		high++;
		swap(D[r], D[high]);

		// край на partition-а

		if (K < low) r = low - 1;
		else if (K == low) return K;
		else if (K < high) (l = low + 1, r = high - 1);
		else if (K == high) return high;
		else l = high + 1;
	}
}

const int MOD = 1000000000;
const int MIN_N = 2000000;
const int RANGE_N = 500000;

int main()
{
	int num = 100;

	double t[4] = { 0 }, s[4] = { 0 }, mn[4] = { 10000, 10000, 10000, 10000 }, mx[4] = { 0 };
	int res[4] = { 0 };

	default_random_engine gen(101);
	uniform_int_distribution<int> RAND_RANGE(MIN_N, MIN_N + RANGE_N);
	uniform_int_distribution<int> RAND_VALUE(0, MOD);

	clock_t cstart, cend;
	int med;

	for (int at = 1; at <= num; at++)
	{
		int n = RAND_RANGE(gen);

		for (int i = 0; i < n; i++)
		{
			A[i] = RAND_VALUE(gen);
			B[i] = A[i];
			C[i] = A[i];
			D[i] = A[i];
		}

		// тест с Median_Of_Medians
		cstart = clock();

		med = med_of_meds(0, n - 1, (n - 1) / 2);
		res[0] = A[med];

		cend = clock();

		t[0] = (double)(cend - cstart) / CLOCKS_PER_SEC * 1000.0;
		if (t[0] < mn[0]) mn[0] = t[0];
		if (t[0] > mx[0]) mx[0] = t[0];

		s[0] += t[0] / num;
		// край

		// тест с Quick_Select
		cstart = clock();

		med = quick_select(0, n - 1, (n - 1) / 2);
		res[1] = B[med];

		cend = clock();

		t[1] = (double)(cend - cstart) / CLOCKS_PER_SEC * 1000.0;
		if (t[1] < mn[1]) mn[1] = t[1];
		if (t[1] > mx[1]) mx[1] = t[1];

		s[1] += t[1] / num;
		// край

		// тест със сортиране
		cstart = clock();

		sort(C, C + n);
		res[2] = C[(n - 1) / 2];

		cend = clock();

		t[2] = (double)(cend - cstart) / CLOCKS_PER_SEC * 1000.0;
		if (t[2] < mn[2]) mn[2] = t[2];
		if (t[2] > mx[2]) mx[2] = t[2];

		s[2] += t[2] / num;
		// край

		// тест с двоен pivot
		cstart = clock();

		med = quick_select_3(0, n - 1, (n - 1) / 2);
		res[3] = D[med];

		cend = clock();

		t[3] = (double)(cend - cstart) / CLOCKS_PER_SEC * 1000.0;
		if (t[3] < mn[3]) mn[3] = t[3];
		if (t[3] > mx[3]) mx[3] = t[3];

		s[3] += t[3] / num;
		// край

		if (res[0] != res[1] || res[1] != res[2] || res[2] != res[3])
			cout << "FATAL ERROR: " << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << endl;

		cout << "Attempt: " << at << ": " << t[0] << " - " << t[1] << " - " << t[2] << " - " << t[3] << endl;
	}

	cout << endl << "Avg Meds: " << s[0] << endl << "Min Meds: " << mn[0] << endl << "Max Meds: " << mx[0] << endl;
	cout << endl << "Avg Quick: " << s[1] << endl << "Min Quick: " << mn[1] << endl << "Max Quick: " << mx[1] << endl;
	cout << endl << "Avg Sort: " << s[2] << endl << "Min Sort: " << mn[2] << endl << "Max Sort: " << mx[2] << endl;
	cout << endl << "Avg Double: " << s[3] << endl << "Min Double: " << mn[3] << endl << "Max Double: " << mx[3] << endl;

	return 0;
}