#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#define N 10000
using namespace std;

typedef vector< vector<int> > Graph;

// помощни масиви
int index[N];
int low_index[N];
bool in_stack[N];

void SCC_Visit(Graph & G, int u, stack<int> & s, int & idx)
{
	index[u] = idx;
	low_index[u] = idx++;

	s.push(u);
	in_stack[u] = true;

	for (int v : G[u]) if (index[v] == -1)
	{
		SCC_Visit(G, v, s, idx);
		low_index[u] = min(low_index[u], low_index[v]);
	}
	else if (in_stack[v]) low_index[u] = min(low_index[u], index[v]);

	int x;
	if (index[u] == low_index[u]) do
	{
		x = s.top();
		s.pop();

		in_stack[x] = false;
		cout << x << " ";
	} while (x != u);
}

void SCC(Graph & G)
{
	int n = G.size();
	int idx = 0;
	stack<int> s;

	for (int i = 0; i < n; i++) index[i] = -1;
	for (int i = 0; i < n; i++) if (index[i] == -1)
	{
		SCC_Visit(G, i, s, idx);
		cout << endl;
	}
}
int main()
{
	Graph g = { { 4 }, { 0 }, { 1, 3 }, { 2 }, { 1 }, { 1, 4, 6 }, { 2, 5 }, { 3, 6 } };
	SCC(g);

	return 0;
}