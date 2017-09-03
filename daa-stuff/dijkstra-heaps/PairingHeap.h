// Pairing Heap for Dijkstra

#include <vector>
using namespace std;

template <class T>
struct PairNode
{
	PairNode *right;
	PairNode *prev; // left or parent
	PairNode *child;
	T key;

	PairNode<T>(const T & newKey){ right = prev = child = NULL; key = newKey; }
};

template <typename T>
class PairingHeap
{
	PairNode<T>* root;
	vector<PairNode<T>*> root_list; // iterative merge_pairs

	int sz;

	PairNode<T>* link(PairNode<T>* H1, PairNode<T>* H2)
	{
		if (!H1) return H2;
		if (!H2) return H1;
		if (H1->key < H2->key) return link(H2, H1);

		H1->prev = H2;
		H1->right = H2->child;
		if (H2->child)(H2->child)->prev = H1;
		H2->child = H1;

		return H2;
	}

	PairNode<T>* merge_pairs_util(PairNode<T>* Rest)
	{
		if (!Rest) return NULL;

		for (PairNode<T>* t = Rest; t;)
		{
			if (!t->right)
			{
				root_list.push_back(t);
				break;
			}

			PairNode<T>* n = t->right->right;
			root_list.push_back(link(t, t->right));
			t = n;
		}

		PairNode<T>* newR = root_list.back();
		root_list.pop_back();

		while (!root_list.empty())
		{
			newR = link(root_list.back(), newR);
			root_list.pop_back();
		}

		return newR;
	}

public:

	PairingHeap(){ root = NULL; sz = 0; }

	PairNode<T>* insert(const T & newKey)
	{
		PairNode<T>* t = new PairNode<T>(newKey);

		root = link(root, t);

		sz++;

		return t;
	}

	T top()
	{
		return root->key;
	}

	void extract_min()
	{
		PairNode<T>* t = root;
		root = merge_pairs_util(root->child);
		if (root)
		{
			root->prev = NULL;
			root->right = NULL;
		}

		delete t;

		sz--;
	}

	int size()
	{
		return sz;
	}

	bool empty()
	{
		return !root;
	}

	void decrease_key(PairNode<T>* H, const T & newKey)
	{
		if (H->key < newKey) return;

		H->key = newKey;

		PairNode<T>* P = H->prev;

		if (!P) return;

		if (H->right) (H->right)->prev = P;

		if (P->child == H)P->child = H->right; else P->right = H->right;

		H->right = H->prev = NULL;

		root = link(root, H);
	}
};
