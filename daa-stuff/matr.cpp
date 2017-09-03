/**
	Най-бързо верижно умножение на n матрици - M(1), M(2), ..., M(n-1)
	В масива a[n] са зададени размерите им: M(i) е с размери a[i-1] x a[i]
	
	Алгоритъмът е демонстриран и с рекурсивна, и с итеративна функция.
	Итеративното изчисление е малко по-бързо откъм реална памет и време.
	И в двата случая обаче алгоритъмът е с асимптотично време O(n^3).
*/

#include <iostream>
#include <algorithm>
#include <vector>
#define N 10000
#define Inf 2000000000;
using namespace std;

// помощни масиви
int D[N][N]; // D[i][j] - оптимално умножение за матриците от i до j
int P[N][N];

int mcm_rec(vector<int>& v, int i, int j)
{
	if (i == j) return 0;
	if (D[i][j] != -1) return D[i][j];
	
	int m = Inf;
	for (int k = i; k < j; k++)
	{
		int t = mcm_rec(v, i, k) + mcm_rec(v, k + 1, j) + v[i - 1] * v[k] * v[j];
		if (t < m)
		{
			m = t;
			P[i][j] = k;
		}
	}

	return D[i][j] = m;
}

int mcm_memo(vector<int> & v)
{
	int n = v.size();
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) D[i][j] = -1;
	
	return mcm_rec(v, 1, n - 1);
}

int mcm_iter(vector<int> & v)
{
	int n = v.size();

	for (int i = 0; i < n; i++) D[i][i] = 0;

	for (int l = 2; l < n; l++) for (int i = 1; i < n - l + 1; i++)
	{
		int j = i + l - 1;
		D[i][j] = Inf;
		for (int k = i; k < j; k++)
		{
			int t = D[i][k] + D[k + 1][j] + v[i - 1] * v[k] * v[j];
			if (t < D[i][j])
			{
				D[i][j] = t;
				P[i][j] = k;
			}
		}
	}

	return D[1][n - 1];
}

void show_par(int i, int j)
{
	if (i == j) cout << "m" << i;
	else
	{
		cout << "(";
		show_par(i, P[i][j]);
		show_par(P[i][j] + 1, j);
		cout << ")";
	}
}

int main()
{
	vector<int> v = { 12, 34, 10, 6, 16, 26, 25, 14, 11, 24 };

	cout << mcm_iter(v) << endl;

	show_par(1, 9);

	cout << endl;

	return 0;
}