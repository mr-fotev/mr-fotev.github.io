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

