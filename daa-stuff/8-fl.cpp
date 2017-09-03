#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#define N 10000
#define Inf 1000000000
using namespace std;

typedef pair <int, int> pr;
typedef vector< vector<pr> > Graph;

// помощни масиви
int dist[N][N];
int path[N][N];

void Floyd_Warshall(Graph & G)
{
	int n = G.size();

	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
	{
		dist[i][j] = Inf;
		path[i][j] = -1;
	}

	for (int u = 0; u < n; u++) dist[u][u] = 0;

	for (int u = 0; u < n; u++) for (pr e : G[u])
	{
		int v = e.first, w = e.second;
		path[u][v] = v;
		dist[u][v] = w;
	}

	for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
	if (dist[i][k] + dist[k][j] < dist[i][j])
	{
		dist[i][j] = dist[i][k] + dist[k][j];
		path[i][j] = path[i][k];
	}
}

void Show_Path(int u, int v)
{
	if (path[u][v] != -1)
	{
		cout << u << " ";
		while (u != v){ u = path[u][v]; cout << u << " "; }
	}
	cout << endl;
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

	Floyd_Warshall(g);

	int n = g.size();

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) if (dist[i][j] != Inf) cout << dist[i][j] << "\t"; else cout << "#\t";
		cout << endl;
	}

	return 0;
}