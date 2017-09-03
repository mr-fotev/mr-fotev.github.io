#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include "PairingHeap.h"
#include "FibonacciHeap.h"
#include "BinaryHeap.h"
#include "BinomialHeap.h"

#define Inf 2000000000
#define N 200000
using namespace std;

typedef pair <int, int> pr;

int n;
int MIN_DEG = 5000;
int DEG_RANGE = 2000;
int MIN_NODES = 20000;
int NODES_RANGE = 5000;
int MAX_W = 30000;

vector <pr> graph[N];

bool finalized[N];
int dist[N];
int parent[N];

FibNode<pr>* fib_places[N];
PairNode<pr>* pr_places[N];
BinNode<pr>* bh_places[N];
BNode<pr>* binom_places[N];
pr h[N];

ofstream f;

clock_t t_start, t_end;

int test_num = 10;

void gen_graph(int n)
{
	for (int i = 0; i < N; i++) graph[i].clear();

	int v, w;

	for (int i = 0; i < n; i++)
	{
		int deg = rand() % DEG_RANGE + MIN_DEG;

		for (int j = 0; j < deg; j++)
		{
			for (v = i; v == i; v = rand() % n);
			w = rand() % MAX_W + 1;
			graph[i].push_back(pr(v, w));
		}
	}
}

void Dijkstra_BinHeap()
{
	double s = 0, t = 0;

	for (int j = 0; j < test_num; j++)
	{
		n = rand() % NODES_RANGE + MIN_NODES;

		gen_graph(n);

		for (int i = 0; i < n; i++){ dist[i] = Inf; finalized[i] = false; parent[i] = -1; h[i] = pr(Inf, i); }
		dist[0] = 0;
		h[0].first = 0;

		t_start = clock();

		BinHeap<pr> q(h, n, bh_places);

		while (!q.empty())
		{
			int u = q.top().second;
			q.extract_min();

			if (dist[u] == Inf) break;
			finalized[u] = true;

			for (pr e : graph[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w + dist[u] < dist[v])
				{
					dist[v] = w + dist[u];
					parent[v] = u;
					q.decrease_key(bh_places[v], pr(dist[v], v));
				}
			}
		}

		t_end = clock();

		t = (t_end - t_start) / (CLOCKS_PER_SEC / 1000.0);

		s += t / test_num;
		cout << "Test " << (j + 1) << ": " << t << endl;
	}

	cout << "AVG: " << s << endl;

	f.open("resultsBH.txt");
	f << s << endl;
	for (int i = 0; i < n; i++) if (dist[i] != Inf) f << dist[i] << "\n"; else f << "INF\n";
	f << endl;
	f.close();
}

void Dijkstra_FibHeap()
{
	double s = 0, t = 0;

	for (int j = 0; j < test_num; j++)
	{
		n = rand() % NODES_RANGE + MIN_NODES;

		gen_graph(n);

		for (int i = 0; i < n; i++){ dist[i] = Inf; finalized[i] = false; parent[i] = -1; }
		dist[0] = 0;

		t_start = clock();

		FibonacciHeap<pr> q;

		for (int i = 0; i < n; i++) fib_places[i] = q.insert(pr(Inf, i));

		q.decrease_key(fib_places[0], pr(dist[0], 0));

		while (!q.empty())
		{
			int u = q.top().second;
			q.extract_min();

			if (dist[u] == Inf) break;
			finalized[u] = true;

			for (pr e : graph[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w + dist[u] < dist[v])
				{
					dist[v] = w + dist[u];
					parent[v] = u;
					q.decrease_key(fib_places[v], pr(dist[v], v));
				}
			}
		}

		t_end = clock();

		t = (t_end - t_start) / (CLOCKS_PER_SEC / 1000.0);

		s += t / test_num;
		cout << "Test " << (j + 1) << ": " << t << endl;
	}

	cout << "AVG: " << s << endl;

	f.open("resultsFH.txt");
	f << s << endl;
	for (int i = 0; i < n; i++) if (dist[i] != Inf) f << dist[i] << "\n"; else f << "INF\n";
	f << endl;
	f.close();
}

void Dijkstra_PairHeap()
{
	double s = 0, t = 0;

	for (int j = 0; j < test_num; j++)
	{
		n = rand() % NODES_RANGE + MIN_NODES;

		gen_graph(n);

		for (int i = 0; i < n; i++){ dist[i] = Inf; finalized[i] = false; parent[i] = -1; }
		dist[0] = 0;

		t_start = clock();

		PairingHeap<pr> q;

		for (int i = 0; i < n; i++) pr_places[i] = q.insert(pr(Inf, i));

		q.decrease_key(pr_places[0], pr(dist[0], 0));

		while (!q.empty())
		{
			int u = q.top().second;
			q.extract_min();

			if (dist[u] == Inf) break;
			finalized[u] = true;

			for (pr e : graph[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w + dist[u] < dist[v])
				{
					dist[v] = w + dist[u];
					parent[v] = u;
					q.decrease_key(pr_places[v], pr(dist[v], v));
				}
			}
		}

		t_end = clock();

		t = (t_end - t_start) / (CLOCKS_PER_SEC / 1000.0);

		s += t / test_num;
		cout << "Test " << (j + 1) << ": " << t << endl;
	}

	cout << "AVG: " << s << endl;

	f.open("resultsPH.txt");
	f << s << endl;
	for (int i = 0; i < n; i++) if (dist[i] != Inf) f << dist[i] << "\n"; else f << "INF\n";
	f << endl;
	f.close();
}

void Dijkstra_PriorQ()
{
	double s = 0, t = 0;

	for (int j = 0; j < test_num; j++)
	{
		n = rand() % NODES_RANGE + MIN_NODES;

		gen_graph(n);

		for (int i = 0; i < n; i++){ dist[i] = Inf; finalized[i] = false; parent[i] = -1; }
		dist[0] = 0;

		t_start = clock();

		priority_queue<pr, vector<pr >, greater<pr> > q;

		q.push(pair<int, int>(dist[0], 0));

		while (!q.empty())
		{
			int u = q.top().second;
			q.pop();

			if (dist[u] == Inf) break;

			if (!finalized[u])
			{
				finalized[u] = true;

				for (pr e : graph[u])
				{
					int v = e.first, w = e.second;
					if (!finalized[v] && w + dist[u] < dist[v])
					{
						parent[v] = u;
						dist[v] = w + dist[u];
						q.push(pair<int, int>(dist[v], v));
					}
				}
			}
		}

		t_end = clock();

		t = (t_end - t_start) / (CLOCKS_PER_SEC / 1000.0);

		s += t / test_num;
		cout << "Test " << (j + 1) << ": " << t << endl;
	}

	cout << "AVG: " << s << endl;

	f.open("resultsPQ.txt");
	f << s << endl;
	for (int i = 0; i < n; i++) if (dist[i] != Inf) f << dist[i] << "\n"; else f << "INF\n";
	f << endl;
	f.close();
}

void Dijkstra_BinomHeap()
{
	double s = 0, t = 0;

	for (int j = 0; j < test_num; j++)
	{
		n = rand() % NODES_RANGE + MIN_NODES;

		gen_graph(n);

		for (int i = 0; i < n; i++){ dist[i] = Inf; finalized[i] = false; parent[i] = -1; }
		dist[0] = 0;

		t_start = clock();

		BinomialHeap<pr> q;

		for (int i = 0; i < n; i++) binom_places[i] = q.insert(pr(Inf, i), i);

		q.decrease_key(binom_places, binom_places[0], pr(dist[0], 0));

		while (!q.empty())
		{
			int u = q.top().second;
			q.extract_min();

			if (dist[u] == Inf) break;
			finalized[u] = true;

			for (pr e : graph[u])
			{
				int v = e.first, w = e.second;
				if (!finalized[v] && w + dist[u] < dist[v])
				{
					dist[v] = w + dist[u];
					parent[v] = u;
					q.decrease_key(binom_places, binom_places[v], pr(dist[v], v));
				}
			}
		}

		t_end = clock();

		t = (t_end - t_start) / (CLOCKS_PER_SEC / 1000.0);

		s += t / test_num;
		cout << "Test " << (j + 1) << ": " << t << endl;
	}

	cout << "AVG: " << s << endl;

	f.open("resultsBmH.txt");
	f << s << endl;
	for (int i = 0; i < n; i++) if (dist[i] != Inf) f << dist[i] << "\n"; else f << "INF\n";
	f << endl;
	f.close();
}

int main(int argc, const char* argv[])
{
	srand(68);
	int H = atoi(argv[1]);

	switch (H)
	{
		case 0: Dijkstra_BinHeap(); break;
		case 1: Dijkstra_PairHeap(); break;
		case 2: Dijkstra_FibHeap(); break;
		case 3: Dijkstra_PriorQ(); break;
		case 4: Dijkstra_BinomHeap(); break;
		default:break;
	}

	return 0;
}