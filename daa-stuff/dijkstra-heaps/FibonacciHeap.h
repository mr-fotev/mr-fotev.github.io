// Fibonacci Heap for Dijkstra

#include <cmath>
using namespace std;

template<class T>
struct FibNode
{
	FibNode *right;
	FibNode *left;
	FibNode *parent;
	FibNode *child;
	T key;
	int degree;
	bool marked;
	FibNode<T>(const T & newKey){ parent = child = NULL; left = right = this; degree = 0; key = newKey; marked = false; }
};

template <typename T>
class FibonacciHeap
{
	FibNode<T>* minimal;
	int sz;

	FibNode<T>* arr[32];

	void insert_before(FibNode<T>* H1, FibNode<T>* H2) // insert H1 before H2 in a root list
	{
		H1->parent = H2->parent;
		H1->right = H2;
		H1->left = H2->left;
		(H2->left)->right = H1;
		H2->left = H1;
	}

	FibNode<T>* link(FibNode<T>* H1, FibNode<T>* H2)
	{
		if (H1->key < H2->key) return link(H2, H1);

		H1->parent = H2;
		H1->marked = false;

		if (H2->child == NULL) H2->child = H1; else insert_before(H1, H2->child);

		H2->degree = H2->degree + 1;

		return H2;
	}

	void merge(FibNode<T>* H1, FibNode<T>* H2)
	{
		(H1->right)->left = H2->left;
		(H2->left)->right = H1->right;
		H1->right = H2;
		H2->left = H1;
	}

	void consolidate()
	{
		FibNode<T>* x = minimal;
		FibNode<T>* t;
		int d;
		do
		{
			d = x->degree;
			t = x->right;
			x->left = x->right = x; // cut x
			while (arr[d] != NULL){ x = link(arr[d], x); arr[d++] = NULL; }
			arr[d] = x;
			x = t;
		} while (x != minimal);

		minimal = NULL;

		int delim = ceil(log2(sz)) + 1;
		for (int i = 0; i <= delim; i++) if (arr[i] != NULL)
		{
			if (!minimal) minimal = arr[i];
			else
			{
				insert_before(arr[i], minimal);
				if (arr[i]->key < minimal->key) minimal = arr[i];
			}
			arr[i] = NULL;
		}
	}

	void cut(FibNode<T>* H1, FibNode<T>* P) // remove H1 from the child list of P
	{
		if (H1->right == H1){ P->child = NULL; P->degree = 0; }
		else
		{
			FibNode<T>* t = H1->left;
			(H1->left)->right = H1->right;
			(H1->right)->left = t;
			P->child = H1->right;
			P->degree = P->degree - 1;
		}
		H1->marked = false;
		insert_before(H1, minimal);
	}

public:
	FibonacciHeap()
	{
		minimal = NULL;
		sz = 0;
	}

	int size()
	{
		return sz;
	}

	bool empty()
	{
		return !minimal;
	}

	FibNode<T>* insert(const T & newKey)
	{
		FibNode<T>* temp = new FibNode<T>(newKey);

		if (!minimal) minimal = temp;
		else
		{
			insert_before(temp, minimal);
			if (temp->key < minimal->key) minimal = temp;
		}

		sz++;

		return temp;
	}

	T top()
	{
		return minimal->key;
	}

	void extract_min()
	{
		FibNode<T>* x = minimal->child;

		if (x) merge(minimal, x);

		if (minimal->right == minimal)
		{
			delete minimal;
			minimal = NULL;
		}
		else
		{
			FibNode<T>* t = minimal->left;
			(minimal->left)->right = minimal->right;
			(minimal->right)->left = t;
			x = minimal->right;
			delete minimal;
			minimal = x;
			consolidate();
		}

		sz--;
	}

	void decrease_key(FibNode<T>* H, T newKey)
	{
		if (H->key < newKey) return;

		FibNode<T>* P = H->parent;
		FibNode<T>* T = H;

		H->key = newKey;

		if (P != NULL && P->key > H->key)
		{
			cut(T, P);

			while (P != NULL && T->marked == true)
			{
				cut(T, P);
				T = P;
				P = T->parent;
			}

			if (P != NULL) T->marked = true;
		}

		if (H->key < minimal->key) minimal = H;
	}
};
