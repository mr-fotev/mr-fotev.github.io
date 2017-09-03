#include <vector>
#include <queue>
#include <iostream>
#define N 10000
#define Inf 2000000000
using namespace std;

typedef vector< vector<int> > Graph;

// помощни масиви
int dist[N];
int parent[N];

void BFS(Graph & G, int s)
{
	int n = G.size();
	for (int i = 0; i < n; i++)
	{
		dist[i] = Inf;
		parent[i] = -1;
	}

	dist[s] = 0;
	parent[s] = s;

	queue<int> Q;
	Q.push(s);

	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();

		for (int v : G[u]) if (parent[v] == -1)
		{
			dist[v] = 1 + dist[u];
			parent[v] = u;
			Q.push(v);
		}
		//  Ако parent[v] != -1 и dist[u] == dist[v], то графът не е двуоцветим.
	}

	parent[s] = -1;
}

// може да се направи и итеративно с помощен масив
void Show_Path(int u)
{
	if (u == -1) return;
	Show_Path(parent[u]);
	cout << u << " ";
}

int main()
{
	// този граф е неориентиран !
	Graph g = { { 2, 3, 4, 7 }, { 2, 3, 5 }, { 0, 1, 8 }, { 0, 1, 6, 7 }, { 0, 6, 8 }, { 1, 7 }, { 3, 4 }, { 0, 3, 5 }, { 2, 4 } };

	BFS(g, 0);

	Show_Path(6);

	cout << endl;

	return 0;
}
