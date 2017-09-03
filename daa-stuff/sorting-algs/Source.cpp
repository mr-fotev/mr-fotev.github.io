#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#define N 10000000
using namespace std;

const int MOD = 1000000;
const int MIN_N = 1000000;
const int RANGE_N = 200000;

int num = 100;

int a[N];
int b[N];

int C[MOD + 1];

/* CombSort */
void comb_sort(int n)
{
	for (int g = n; g >= 10; g = g * 10 / 13) for (int i = 0; i + g < n; i++) if (a[i] > a[i + g]) swap(a[i], a[i + g]);

	for (int i = 1; i < n; i++)
	{
		int temp = a[i], j;
		for (j = i; j > 0 && a[j - 1] > temp; j--) a[j] = a[j - 1];
		a[j] = temp;
	}
}

/* ShellSort */
int gaps[14] = { 100894, 44842, 19930, 8858, 3937, 1750, 701, 301, 132, 57, 23, 10, 4, 1 };

void shell_sort(int n)
{
	for (int k = 0; k < 14; k++)
	{
		int g = gaps[k];
		for (int i = g; i < n; i++)
		{
			int temp = a[i], j;
			for (j = i; j >= g && a[j - g] > temp; j -= g) a[j] = a[j - g];
			a[j] = temp;
		}
	}
}

/* CountingSort */
void counting_sort(int n)
{
	int i;

	for (i = 0; i <= MOD; i++) C[i] = 0;
	for (i = 0; i < n; i++) C[a[i]]++;
	for (i = 1; i <= MOD; i++) C[i] += C[i - 1];
	for (i = n - 1; i >= 0; i--) b[--(C[a[i]])] = a[i];
	for (i = 0; i < n; i++) a[i] = b[i];
}

/* HeapSort */
int heap_size;

void heapify(int i)
{
	int node = a[i];
	for (bool corr = true; corr;)
	{
		int l = 2 * i + 1, r = l + 1, m = r < heap_size ? (a[l] > a[r] ? l : r) : (l < heap_size ? l : i);
		if (m != i && a[m] > node) i = (a[i] = a[m], m); else corr = false;
	}
	a[i] = node;
}

void heap_sort(int n)
{
	heap_size = n;
	for (int i = n / 2 - 1; i >= 0; i--) heapify(i);
	for (int i = n - 1; i >= 1; i--)
	{
		swap(a[0], a[i]);
		heap_size--;
		heapify(0);
	}
}

/* MergeSort */
void merge(int l, int m, int r)
{
	int i = l, j = m + 1, p = 0;

	while (i <= m && j <= r) b[p++] = (a[i] > a[j] ? a[j++] : a[i++]);

	if (i > m) while (j <= r) b[p++] = a[j++]; else while (i <= m) b[p++] = a[i++];

	for (int k = l; k <= r; k++) a[k] = b[k - l];
}

void merge_sort(int l, int r)
{
	if (l < r)
	{
		int m = (l + r) / 2;
		merge_sort(l, m);
		merge_sort(m + 1, r);
		merge(l, m, r);
	}
}

/* QuickSort */
int partition(int l, int r)
{
	swap(a[r], a[(l + r) / 2]);
	int indx = l;

	for (int i = l; i < r; i++) if (a[i] < a[r]) swap(a[i], a[indx++]);

	swap(a[r], a[indx]);
	return indx;
}

void quick_sort(int l, int r)
{
	if (l < r)
	{
		int m = partition(l, r);
		quick_sort(l, m - 1);
		quick_sort(m + 1, r);
	}
}

int main()
{
	double s = 0, mn = 1000, mx = 0;
	double t;

	default_random_engine gen(56);
	uniform_int_distribution<int> RAND_RANGE(MIN_N, MIN_N + RANGE_N);
	uniform_int_distribution<int> RAND_VALUE(0, MOD);

	for (int at = 1; at <= num; at++)
	{
		int n = RAND_RANGE(gen);

		for (int i = 0; i < n; i++) a[i] = RAND_VALUE(gen);

		clock_t start = clock();
		//quick_sort(0, n - 1);
		//merge_sort(0, n - 1);
		//heap_sort(n);
		counting_sort(n);
		//comb_sort(n);
		//shell_sort(n);
		//sort(a ,a + n);
		clock_t end = clock();
		
		t = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
		if (t < mn) mn = t;
		if (t > mx) mx = t;
		
		s += t / num;
		
		cout << "Attempt: " << at << ": " << t << endl;
	}

	cout << endl << "Avg: " << s << endl << "Min: " << mn << endl << "Max: " << mx << endl;
	
	return 0;
}