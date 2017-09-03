// 0_1 knapsack

#include <iostream>
#include <algorithm>
#include <vector>
#define N 10000
#define MaxCap 10000
using namespace std;

typedef pair<int, int> pr;

// таблица и масив за конкретните обекти
int D[N][MaxCap]; // D[i][j] := максимална печалба за елементите от i нататък за капацитет j
pr El[N];
int IND;

int knapsack(vector<pr> & v, int c)
{
	int n = v.size();

	for (int j = 0; j <= c; j++) D[n][j] = 0;
	for (int i = n - 1; i >= 0; i--) for (int j = 0; j <= c; j++)
	{
		int val = v[i].first, cap = v[i].second;
		D[i][j] = D[i + 1][j];
		if (cap <= j) D[i][j] = max(D[i + 1][j], D[i + 1][j - cap] + val);
	}

	IND = 0;
	for (int w = c, i = 0; i < n; i++)
	{
		int val = v[i].first, cap = v[i].second;
		if (cap <= w && D[i][w] == D[i + 1][w - cap] + val)
		{
			El[IND++] = v[i];
			w -= cap;
		};
	}

	return D[0][c];
}

int main()
{
	vector<pr> v = { pr(7, 1), pr(13, 2), pr(1, 6), pr(12, 8), pr(8, 4), pr(6, 3), pr(5, 7), pr(14, 6), pr(4, 5), pr(2, 6) };

	cout << knapsack(v, 27) << endl;

	for (int i = 0; i < IND; i++) cout << "(" << El[i].first << " - " << El[i].second << ") ";
	cout << endl;

	return 0;
}