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
int key[N];
int parent[N];

int MST_Prim(Graph & G, int s)
{
	int n = G.size();

	for (int i = 0; i < n; i++)
	{
		key[i] = Inf;
		finalized[i] = false;
		parent[i] = -1;
	}

	priority_queue<pr, vector<pr>, greater<pr> > Q; // minHeap

	Q.push(pr(0, s));
	key[s] = 0;

	int sum = 0;

	while (!Q.empty())
	{
		int u = Q.top().second;
		Q.pop();

		if (!finalized[u])
		{
			finalized[u] = true;
			sum += key[u];
			for (pr e : G[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w < key[v])
				{
					parent[v] = u;
					key[v] = w;
					Q.push(pr(key[v], v));
				}
			}
		}
	}

	return sum;
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
	
	cout << "MPD_Sum = " << MST_Prim(g, 0) << endl;

	return 0;
}