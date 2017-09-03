#include <iostream>
#include <algorithm>
#define N 10000
using namespace std;

class UFS // Union-Find структура с фиксиран брой елементи
{
	int size;
	int* parent;
	int* rank;

public:
	UFS(int s)
	{
		parent = new int[s];
		rank = new int[s];
		size = s;
		for (int i = 0; i < s; i++)
		{
			parent[i] = i;
			rank[i] = 0;
		}
	}

	int find(int x)
	{
		if (parent[x] != x) parent[x] = find(parent[x]);
		return parent[x];
	}

	void unite(int x, int y)
	{
		int rx = find(x);
		int ry = find(y);

		if (rx != ry)
		{
			if (rank[rx] > rank[ry]) parent[ry] = rx;
			else if (rank[rx] < rank[ry]) parent[rx] = ry;
			else
			{
				parent[ry] = rx;
				rank[rx]++;
			}
		}
	}
};

struct edge
{
	int u, v, w;
	edge() :u(0), v(0), w(0){}
	edge(int u_, int v_, int w_) :u(u_), v(v_), w(w_){}
	edge& operator = (edge& e){ u = e.u; v = e.v; w = e.w; return *this; }
	bool operator < (const edge &e) const { return w < e.w; }
};

// помощен масив за резултата
edge MPD_Result[N];

int MST_Kruskal(edge * G, int n, int m)
{
	sort(G, G + m);

	UFS S(n);
	int sum = 0;
	int c = 0;

	for (int i = 0; i < m && c < n - 1; i++)
	{
		edge & e = G[i];
		if (S.find(e.u) != S.find(e.v))
		{
			sum += e.w;
			MPD_Result[c++] = e;
			S.unite(e.u, e.v);
		}
	}

	return sum;
}

int main()
{
	const int n = 8, m = 11;

	edge G[m] = {
		edge(0, 1, 1), edge(0, 3, 3), edge(0, 4, 5),
		edge(1, 2, 2), edge(1, 5, 2),
		edge(2, 4, 6),
		edge(3, 6, 10),
		edge(4, 6, 7), edge(4, 7, 2),
		edge(5, 7, 5), edge(6, 7, 20) }; // неориентиран граф като списък от ребра

	cout << "MPD_Sum = " << MST_Kruskal(G, n, m) << endl;

	return 0;
}