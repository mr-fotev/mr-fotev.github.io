// subset_sum

#include <iostream>
#include <algorithm>
#include <vector>
#define N 10000
#define MaxS 10000
using namespace std;

// таблица и масив за конкретните обекти
bool D[N][MaxS]; // D[i][j] := има ли подредица за елементите от i нататък със сума j
int El[N];
int IND;

bool subset_sum(vector<int> & v, int s)
{
	int n = v.size();

	D[n][0] = true;
	for (int j = 1; j <= s; j++) D[n][j] = false;

	for (int i = n - 1; i >= 0; i--) for (int j = 0; j <= s; j++)
	{
		D[i][j] = D[i + 1][j];
		if (v[i] <= j) D[i][j] = D[i + 1][j] || D[i + 1][j - v[i]];
	}

	IND = 0;
	if (D[0][s])
	{
		for (int w = s, i = 0; i < n; i++) if (v[i] <= w && D[i + 1][w - v[i]])
		{
			El[IND++] = v[i];
			w -= v[i];
		}
	}

	return D[0][s];
}

int main()
{
	vector<int> v = { 7, 13, 1, 12, 9, 6, 5, 14, 4, 8 };

	cout << subset_sum(v, 29) << endl;

	for (int i = 0; i < IND; i++) cout << El[i] << " ";
	cout << endl;

	return 0;
}