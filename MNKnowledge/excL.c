#include <stdio.h>
#define N 20001

long long p[N][13];
int i, j, s;

int main(){
    while (j < 13){
        p[0][j++] = 1;
    }
    
	for (i = 1; i < N; i++){
		p[i][12] = (i%10000 == 0);
		for (j = 11, s = 5000; j >= 0; j--){
			p[i][j] = p[i][j+1] + (s <= i ? p[i-s][j]:0);
			s = (s == 50 || s == 500 || s == 5000 ? 2*s/5:s/2);
		}
	}
	
	while (scanf("%d", &s) > 0){
        printf("%lld\n", p[s][0]);
    }
    
    return 0;
}