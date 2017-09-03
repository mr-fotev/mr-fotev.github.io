#include <vector>
#include <iostream>
#define N 10000
using namespace std;

typedef vector< vector<int> > Graph;
enum color{ white, gray, black };

// помощни масиви
int found[N];
int finalized[N];
int parent[N];
color colors[N];

void DFS_visit(Graph & G, int u, int & time)
{
	colors[u] = gray;
	found[u] = ++time;

	for (int v : G[u]) if (colors[v] == white)
	{
		parent[v] = u;
		DFS_visit(G, v, time);
	}
	/*
	Ако colors[v] == gray и графът е ориентиран, то в него има цикъл.
	За неориентиран граф, трябва да се провери дали v e родител на u - ако да,
	то това не се брой за цикъл, ако не - има цикъл.
	*/

	colors[u] = black;
	finalized[u] = ++time;
}

void DFS(Graph & G)
{
	int n = G.size();

	for (int i = 0; i < n; i++)
	{
		colors[i] = white;
		parent[i] = -1;
	}

	int time = 0;
	for (int i = 0; i < n; i++) if (colors[i] == white) DFS_visit(G, i, time);
}

int main()
{
	// този граф е неориентиран !
	Graph g = { { 2, 3, 4, 7 }, { 2, 3, 5 }, { 0, 1, 8 }, { 0, 1, 6, 7 }, { 0, 6, 8 }, { 1, 7 }, { 3, 4 }, { 0, 3, 5 }, { 2, 4 } };

	DFS(g);

	int n = g.size();

	for (int i = 0; i < n; i++) cout << "[" << i << ": " << found[i] << ", " << finalized[i] << "] ";
	cout << endl;

	return 0;
}