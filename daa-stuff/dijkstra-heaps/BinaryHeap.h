// Binary Heap for Dijkstra

#include <vector>
#include <algorithm>
using namespace std;

template <class T>
struct BinNode
{
	T key;
	int pos;

	BinNode(T newKey, int newPos){ key = newKey; pos = newPos; }
};

template<typename T>
class BinHeap
{
	vector<BinNode<T>*> heap;
	int sz;

	void set_pos(int i, BinNode<T>* H)
	{
		heap[i] = H;
		heap[i]->pos = i;
	}

	int heapify(T key, int i)
	{
		int left = 2 * i, right = 2 * i + 1;

		int s = i;
		if (left <= sz && heap[left]->key < key) s = left;
		if (right <= sz && heap[right]->key < min(key, heap[left]->key)) s = right;
		if (s == i) return s;

		heap[i] = heap[s];
		heap[i]->pos = i;

		return heapify(key, s);
	}

public:
	BinHeap()
	{
		heap.push_back(NULL); // unused delimiter
		sz = 0;
	}

	BinHeap(T a[], int s, BinNode<T>* bh_places[]) // for Dijsktra
	{
		heap.reserve(s + 1);
		heap.push_back(NULL);
		for (int i = 0; i < s; i++)
		{
			heap.push_back(new BinNode<T>(a[i], i + 1));
			bh_places[i] = heap.back();
		}

		sz = s;

		for (int i = sz / 2; i > 1; i--)
		{
			BinNode<T>* temp = heap[i];

			int indx = heapify(temp->key, i);

			set_pos(indx, temp);
		} // build_heap
	}

	int size()
	{
		return sz;
	}

	bool empty()
	{
		return sz == 0;
	}

	BinNode<T>* insert(T newKey)
	{
		BinNode<T>* t = new BinNode<T>(newKey, ++sz);
		heap.push_back(t);

		int i = sz;
		while (i > 1 && heap[i / 2]->key > newKey)
		{
			set_pos(i, heap[i / 2]);
			i /= 2;
		}

		set_pos(i, t);

		return t;
	}

	T top()
	{
		return heap[1]->key;
	}

	void extract_min()
	{
		set_pos(1, heap[sz--]);

		heap.pop_back();

		BinNode<T>* temp = heap[1];

		int indx = heapify(temp->key, 1);

		set_pos(indx, temp);
	}

	void decrease_key(BinNode<T>* H, T newKey)
	{
		if (H->key < newKey) return;

		H->key = newKey;

		int i = H->pos;
		while (i > 1 && heap[i / 2]->key > newKey)
		{
			set_pos(i, heap[i / 2]);
			i /= 2;
		}

		set_pos(i, H);
	}
};
