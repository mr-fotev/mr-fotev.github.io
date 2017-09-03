#include <vector>
#include <iostream>
#define N 10000
using namespace std;

typedef vector< vector<int> > Graph;

// помощни масиви
bool visited[N];
int rev_finalized[N];

void DFS_visit(Graph & G, int u, int & idx)
{
	visited[u] = true;
	for (int v : G[u]) if (!visited[v]) DFS_visit(G, v, idx);
	rev_finalized[idx--] = u;
}

void SCC_visit(Graph & G, int u)
{
	visited[u] = true;

	cout << u << " ";

	for (int v : G[u]) if (!visited[v]) SCC_visit(G, v);
}

void SCC(Graph & G)
{
	int n = G.size();
	int idx = n - 1;

	// DFS на началния граф
	for (int i = 0; i < n; i++) visited[i] = false;
	for (int i = 0; i < n; i++) if (!visited[i]) DFS_visit(G, i, idx);

	// строене на транспонирания граф
	Graph T;
	for (int i = 0; i < n; i++) T.push_back({});
	for (int i = 0; i < n; i++) for (int u : G[i]) T[u].push_back(i);

	// силно свързани компоненти
	for (int i = 0; i < n; i++) visited[i] = false;

	for (int i = 0; i < n; i++) if (!visited[rev_finalized[i]])
	{
		SCC_visit(T, rev_finalized[i]);
		cout << endl;
	}
}

int main()
{
	Graph g = { { 4 }, { 0 }, { 1, 3 }, { 2 }, { 1 }, { 1, 4, 6 }, { 2, 5 }, { 3, 6 } };
	SCC(g);

	return 0;
}