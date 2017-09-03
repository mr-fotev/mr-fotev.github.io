// Тази реализация не ползва decrease_key

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
bool finalized[N];
int dist[N];
int parent[N];

void Dijkstra(Graph & G, int s)
{
	int n = G.size();

	for (int i = 0; i < n; i++)
	{
		dist[i] = Inf;
		finalized[i] = false;
		parent[i] = -1;
	}

	priority_queue<pr, vector<pr>, greater<pr> > Q; // minHeap

	Q.push(pr(0, s));
	dist[s] = 0;

	while (!Q.empty())
	{
		int u = Q.top().second;
		Q.pop();

		if (!finalized[u])
		{
			finalized[u] = true;

			for (pr e : G[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w + dist[u] < dist[v])
				{
					parent[v] = u;
					dist[v] = w + dist[u];
					Q.push(pr(dist[v], v));
				}
			}
		}
	}
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

	Dijkstra(g, 0);

	int n = g.size();

	for (int i = 0; i < n; i++) if (dist[i] != Inf) cout << dist[i] << " "; else cout << "INF ";
	cout << endl;

	return 0;
}