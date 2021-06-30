/*
[28-Jun-21 11:19]

Задача: Дин
Дин е игра за един човек, в която играчът започва с три кутии с топчета (или други еднакви предмети, като камъчета или карти).
Единственият валиден ход в играта е прехвърляне на топчета от една кутия в друга, така че количеството в целевата кутия да се удвои.
Целта на играта е една от кутиите да остане празна.

Нека за удобство кръстим кутиите A, B и C и да разгледаме една примерна игра:

A	B	C	Ход
1	2	3	
			C→B
1	4	1	
			C→A
2	4	0	

Кутия C е празна и играта е спечелена.

Съществуват и други начини да се спечели тази конкретна конфигурация.
Например първи ход C→A ще остави по две топчета във всяка кутия и всеки следващ ход ще е печеливш.

Напишете програма, която играе Дин. На стандартния вход програмата трябва получи три цели числа разделени със спейс A, B и C (0 < A < B < C < 100 000)
– съответно количеството топчета във всяка от кутиите. На стандартния изход програмата трябва да изведе ходове, всеки на нов ред, с формат
<кутия източник>-><кутия цел>. Ако играта е невъзможно да бъде спечелена, програмата трябва да изпише NOT POSSIBLE на стандартния изход.

Пример

Вход:
1 2 3

Изход:
C->B
C->A

ПП Напомняме че това не е единственият валиден изход за този вход
*/

// Reduce min strategy

#include <iostream>
#include <ctime>
using namespace std;

int a, b, c;

void reduce(int& iLeft, int& iMid, int& iRight){
	int iDiv = iMid / iLeft;

	while (iDiv > 0){
		cout << a << "-" << b << "-" << c << endl;

		if (iDiv % 2) iMid -= iLeft;
		else iRight -= iLeft;

		iDiv /= 2;
		iLeft *= 2;
	}
}

int main(){
	cin >> a >> b >> c;

	clock_t start = clock();

	while (true){
		if (a == 0 || b == 0 || c == 0){
			cout << a << "-" << b << "-" << c << endl;
			break;
		}
		
		if (a <= b){
			if (c <= a) reduce(c, a, b);
			else if (b <= c) reduce(a, b, c);
			else reduce(a, c, b);
		}
		else{
			if (c <= b) reduce(c, b, a);
			else if (a <= c) reduce(b, a, c);
			else reduce(b, c, a);
		}
	}

	clock_t end = clock();

	cout << endl << (double)(end - start) / CLOCKS_PER_SEC;

	return 0;
}

// Pure BFS strategy

#include <iostream>
#include <ctime>
#include <vector>
#define N 524288
#define MOD 100019

using namespace std;

vector <long long> vKeys[MOD];
vector <long long> vVals[MOD];

long long get(long long key){
	long long h = key % MOD;
	
	for (int i = 0; i < vKeys[h].size(); i++){
		if (vKeys[h][i] == key){
			return vVals[h][i];
		}
	}
	
	return -1;
}

void set(long long key, long long val){
	long long h = key % MOD;
	vKeys[h].push_back(key);
	vVals[h].push_back(val);
}

vector <long long> vParents;
vector <long long> vNodes;
int iFront = 0;

long long sq;
long long p;

void place_next(long long a, long long b){
	vParents.push_back(sq);
	vNodes.push_back(N*a+b);
}

int main(){
	long long a, b, c;

	cin >> a >> b >> c;
	
	clock_t start = clock();
	
	long long sum = a + b + c;
	
	vParents.push_back(-2);
	vNodes.push_back(N*a+b);
	
	while (true){
		p = vParents[iFront];
		sq = vNodes[iFront];
		
		iFront++;
		
		if (get(sq) != -1) continue;
		
		a = sq / N;
		b = sq % N;
		c = sum - a - b;

		if (a == 0 || b == 0 || c == 0){
			set(sq, p);
			break;
		}
		
		if (a >= b) place_next(a-b, 2*b);
		if (a >= c) place_next(a-c, b);
		if (b >= a) place_next(2*a, b-a);
		if (b >= c) place_next(a, b-c);
		if (c >= a) place_next(2*a, b);
		if (c >= b) place_next(a, 2*b);
		
		set(sq, p);
	}
	
	iFront = 0;
	
	while (sq != -2){
		vNodes[iFront] = sq;
		iFront++;
		sq = get(sq);
	}

	for (int i = iFront-1; i >= 0; i--){
		a = vNodes[i] / N;
		b = vNodes[i] % N;
		c = sum - a - b;
		
		cout << a << "-" << b << "-" << c << endl;
	}

	clock_t end = clock();

	cout << endl << (double)(end - start) / CLOCKS_PER_SEC;

	return 0;
}

