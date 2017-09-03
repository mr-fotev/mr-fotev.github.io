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
bool visited[N];
int dist[N];
int parent[N];
int topo_order[N];

void Topo_visit(Graph & G, int u, int & idx)
{
	visited[u] = true;
	for (pr e : G[u]) if (!visited[e.first]) Topo_visit(G, e.first, idx);
	topo_order[idx--] = u;
}

void DAG_Shortest_paths(Graph & G, int s)
{
	int n = G.size();
	int idx = n - 1;

	// топологично сортиране
	for (int i = 0; i < n; i++) visited[i] = false;
	for (int i = 0; i < n; i++) if (!visited[i]) Topo_visit(G, i, idx);

	// най-кратки пътища
	for (int i = 0; i < n; i++)
	{
		dist[i] = Inf;
		parent[i] = -1;
	}

	dist[s] = 0;

	for (int i = 0; i < n; i++)
	{
		int u = topo_order[i];

		if (dist[u] != Inf) for (pr e : G[u])
		{
			int v = e.first, w = e.second;
			if (dist[u] + w < dist[v])
			{
				dist[v] = dist[u] + w;
				parent[v] = u;
			}
		}
	}
}

int main()
{
	Graph g = {
		{ pr(1, 1), pr(3, 3), pr(4, 5) },
		{ pr(2, 2), pr(5, 2) },
		{},
		{ pr(6, 10) },
		{ pr(7, 2) },
		{},
		{ pr(7, 1) },
		{ pr(5, 5) }
	};

	DAG_Shortest_paths(g, 0);

	int n = g.size();

	for (int i = 0; i < n; i++) if (dist[i] != Inf) cout << dist[i] << " "; else cout << "INF ";
	cout << endl;

	return 0;
}