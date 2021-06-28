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
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>

using namespace std;

unordered_map<string, string> P;

struct State{
	int l, m, r;
	State(int a, int b, int c){ l = a; m = b; r = c; }
	string str(){ return to_string(l) + "-" + to_string(m) + "-" + to_string(r); }
};

queue<State> Q;
string sq;

void place_next(int a, int b, int c){
	State n(a, b, c);
	string sn = n.str();

	if (P.find(sn) == P.end()){
		Q.push(n);
		P[sn] = sq;
	}
}

int main(){
	int a, b, c;

	cin >> a >> b >> c;
	
	clock_t start = clock();
	
	State s(a, b, c);

	Q.push(s);
	P[s.str()] = "";

	sq = "";

	while (true){
		State q = Q.front();
		sq = q.str();

		Q.pop();

		if (q.l * q.m * q.r == 0) break;
		
		if (q.l >= q.m) place_next(q.l - q.m, 2 * q.m, q.r);
		if (q.l >= q.r) place_next(q.l - q.r, q.m, 2 * q.r);
		if (q.m >= q.l) place_next(2 * q.l, q.m - q.l, q.r);
		if (q.m >= q.r) place_next(q.l, q.m - q.r, 2 * q.r);
		if (q.r >= q.l) place_next(2 * q.l, q.m, q.r - q.l);
		if (q.r >= q.m) place_next(q.l, 2 * q.m, q.r - q.m);
	}
	
	vector<string> path;

	while (sq != ""){
		path.push_back(sq);
		sq = P[sq];
	}

	for (int i = path.size() - 1; i >= 0; i--) cout << path[i] << endl;

	clock_t end = clock();

	cout << endl << (double)(end - start) / CLOCKS_PER_SEC;

	return 0;
}
