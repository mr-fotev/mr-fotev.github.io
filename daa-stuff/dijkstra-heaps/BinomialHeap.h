// Binomial Heap for Dijkstra

#include <vector>
using namespace std;

template <class T>
struct BNode
{
	T key;
	BNode* parent;
	BNode* right;
	BNode* child;

	int node;
	int deg;

	BNode(T k, int gn){ key = k; parent = right = child = NULL; deg = 0; node = gn; }
};

template <typename T>
class BinomialHeap
{
	BNode<T>* root;
	BNode<T>* min;

	vector<BNode<T>*> v; // for merge

	int sz;

	BNode<T>* link(BNode<T>* H1, BNode<T>* H2)
	{
		if (H1->key < H2->key) return link(H2, H1);

		H1->parent = H2;
		H1->right = H2->child;
		H2->child = H1;
		H2->deg = H2->deg + 1;

		return H2;
	}

	void push_to_v(BNode<T>* H) //push H to v and turn right in H
	{
		if (v.empty() || v.back()->deg < H->deg) v.push_back(H);
		else
		{
			BNode<T>* temp = v.back();
			v.pop_back();
			v.push_back(link(temp, H));
		}
	}

	void merge(BNode<T>* L1, BNode<T>* L2)
	{
		BNode<T>* Iter1 = L1, *Iter2 = L2;

		while (Iter1 && Iter2)
		{
			if (Iter1->deg < Iter2->deg)
			{
				BNode<T>* r = Iter1->right;
				push_to_v(Iter1);
				Iter1 = r;
			}
			else if (Iter1->deg > Iter2->deg)
			{
				BNode<T>* r = Iter2->right;
				push_to_v(Iter2);
				Iter2 = r;
			}
			else
			{
				BNode<T>* r1 = Iter1->right, *r2 = Iter2->right, *temp = link(Iter1, Iter2);
				push_to_v(temp);
				Iter1 = r1;
				Iter2 = r2;
			}
		}

		if (Iter1) while (Iter1)
		{
			BNode<T>* r = Iter1->right;
			push_to_v(Iter1);
			Iter1 = r;
		}

		if (Iter2) while (Iter2)
		{
			BNode<T>* r = Iter2->right;
			push_to_v(Iter2);
			Iter2 = r;
		}

		root = v[0];
		min = v[0];

		BNode<T>* temp = root;
		for (int i = 1; i < (int)v.size(); i++)
		{
			temp->right = v[i];
			if (v[i]->key < min->key) min = v[i];
			temp = temp->right;
		}

		temp->right = NULL;

		v.clear();
	}

public:
	BinomialHeap(){ root = min = NULL; sz = 0; }

	BNode<T>* insert(T key, int n)
	{
		BNode<T>* temp = new BNode<T>(key, n);
		merge(temp, root);
		sz++;
		return temp;
	}

	T top()
	{
		return min->key;
	}

	void extract_min()
	{
		// reverse the child of min
		for (BNode<T>* temp = min->child; temp; temp = temp->right) v.push_back(temp);
		BNode<T>* RChild = NULL;

		if (!v.empty())
		{
			RChild = v.back();
			v.pop_back();

			BNode<T>* temp = RChild;
			while (!v.empty())
			{
				temp->right = v.back();
				temp->parent = NULL;
				v.pop_back();
				temp = temp->right;
			}
			temp->right = NULL;
			temp->parent = NULL;
		}

		// find min and delete from root_list
		if (min == root) root = root->right;
		else
		{
			BNode<T>* pre_min = root;
			while (pre_min->right != min) pre_min = pre_min->right;
			pre_min->right = min->right;
		}

		delete min;

		if (!root && !RChild) root = min = NULL;
		else merge(RChild, root);

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

	void decrease_key(BNode<T>* L[], BNode<T>* H, T newKey)
	{
		if (newKey > H->key) return;

		BNode<T>* Iter = H, *P = Iter->parent;
		int nd = H->node;

		while (P && P->key > newKey)
		{
			L[P->node] = Iter;
			Iter->key = P->key;
			Iter->node = P->node;

			Iter = P;
			P = P->parent;
		}

		L[nd] = Iter;
		Iter->key = newKey;
		Iter->node = nd;

		if (Iter->key < min->key) min = Iter;
	}
};
