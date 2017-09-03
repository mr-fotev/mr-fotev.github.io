#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#define N 10000
#define Inf 2000000000
using namespace std;

typedef pair <int, int> pr;
typedef vector< vector<pr> > Graph;

// помощни масиви
int dist[N];
int parent[N];

bool Bellman_Ford(Graph & G, int s)
{
	int n = G.size();

	for (int i = 0; i < n; i++)
	{
		dist[i] = Inf;
		parent[i] = -1;
	}

	dist[s] = 0;

	for (int k = 1; k < n; k++) for (int u = 0; u < n; u++) for (pr e : G[u])
	{
		int v = e.first, w = e.second;
		if (w + dist[u] < dist[v])
		{
			parent[v] = u;
			dist[v] = w + dist[u];
		}
	}

	for (int u = 0; u < n; u++) for (pr e : G[u])
	{
		int v = e.first, w = e.second;
		if (w + dist[u] < dist[v]) return false;
	}

	return true;
}

int main()
{
	Graph g = {
		{ pr(1, 1), pr(3, 3), pr(4, 5) },
		{ pr(0, 1), pr(2, 2), pr(5, 2) },
		{ pr(1, 2), pr(4, 6) },
		{ pr(0, 3), pr(6, 10) },
		{ pr(0, 5), pr(2, 6), pr(6, 7), pr(7, 2) },
		{ pr(1, 2), pr(7, 5) },
		{ pr(3, 10), pr(4, 7), pr(7, 20) },
		{ pr(4, 2), pr(5, 5), pr(6, 20) } };

	Bellman_Ford(g, 0);

	int n = g.size();
	
	for (int i = 0; i < n; i++) if (dist[i] != Inf) cout << dist[i] << " "; else cout << "INF ";
	cout << endl;

	return 0;
}