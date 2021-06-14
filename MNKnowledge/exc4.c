/* 
[14-Jun-21 13:08]

Задача 1. Къде е точката?
Напишете на езика C или C++ програма, която по зададени върхове на триъгълник и точка определя къде спрямо триъгълника
се намира точката. От стандартния вход се въвеждат координатите на върховете A, B и C и на точката P – цели числа.
На стандартния изход се извежда някой от низовете: A, B или C, ако P съвпада със съответния връх; AB, BC или CA, ако
P лежи на съответната страна и не е връх; ABC, ако P е вътрешна и -ABC, ако P е външна за триъгълника.

Задача 2. Спирала
Напишете на езика C или C++ програма, която обхожда клетките на правоъгълна мрежа по спирала, отвън навътре в посока на часовника:
отначало тези от първия ред отляво надясно, после тези от последния стълб отгоре надолу, тези от последния ред отдясно наляво и т.н.
до изчерпване на клетките. От стандартния вход се въвеждат броя редове m и броя стълбове n на таблицата – цели положителни числа.
На стандартния изход се извеждат координатите на клетките по реда на посещаването им – за всяка клетка на отделен ред.
Клетките от първия ред на мрежата имат координати 1 1, 1 2, . . . , 1 n, тези от втория – 2 1, 2 2, . . . 2 n и т. н.
Например за въведено 3 3 трябва да се изведат редовете 1 1, 1 2, 1 3, 2 3, 3 3, 3 2, 3 1, 2 1 и 2 2.
*/

// #NoGolf1

#include <stdio.h>

int a[8];
int c(int pa, int pb){ return a[2*pa]*a[2*pb+1]-a[2*pb]*a[2*pa+1]; }
int det(int pa, int pb, int pc){ return c(pa,pb)+c(pb,pc)+c(pc,pa); }

int main(){
	int i;	
	for (i=0; i<8; i++) scanf("%d", a+i);

	int sideAB = det(3,0,1);
	int sideBC = det(3,1,2);
	int sideCA = det(3,2,0);
	
	int bInside = ((sideAB <= 0) && (sideBC <= 0) && (sideCA <= 0)) || ((sideAB >= 0) && (sideBC >= 0) && (sideCA >= 0));
	
	if (!sideAB && !sideCA) printf("A");
	else if (!sideAB && !sideBC) printf("B");
	else if (!sideBC && !sideCA) printf("C");
	else if (!sideAB && bInside) printf("AB");
	else if (!sideBC && bInside) printf("BC");
	else if (!sideCA && bInside) printf("AC");
	else if (bInside) printf("ABC");
	else printf("-ABC");
	
	return 0;
}

// #NoGolf2

#include<stdio.h>

int main(){
	int n,m,k,r,c,i;
	
	scanf("%d%d",&m,&n);
	
	for (k=1,r=m,c=n; k<=r && k<=c; k++,r--,c--){
		if (k<=r) for (i=k; i<=c; i++) printf("%d %d\n",k,i);
		if (k<=c) for (i=k+1; i<=r; i++) printf("%d %d\n",i,c);
		if (k<r) for (i=c-1; i>=k; i--) printf("%d %d\n",r,i);
		if (k<c) for (i=r-1; i>k; i--) printf("%d %d\n",i,k);
	}
	
	return 0;
}
